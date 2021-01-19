/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_bmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 21:05:48 by eniini            #+#    #+#             */
/*   Updated: 2020/12/03 17:15:07 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Creates a bitmap image file based on the current [fdf->img] array.
**	Currently the image is created as a 1:1 copy without any compression.
**	Metadata headers have a set size:
**			BITMAP_FILE_HEADER = 14 bytes
**			BITMAP_INFO_HEADER = 40 bytes
**	(later one is chosen for its support of larger bpp formats)
**	Check [fileformat.info/format/bmp/egff.htm] for documentation.
**
**	Note to self: BMP headers store values (larger than 1 byte) in
**	little-endian order.
*/

#include "fdf.h"

/*
**	BITMAP_FILE_HEADER contents:
**	'BM' is the file signature/magic bytes corresponding to .BMP
**	Next 4 bytes are used to represent file's size in bytes.
**	Final 4 bytes indicate the offset between metadata & pixel data.
*/

static unsigned char	*cr_bmap_fileheader(int h, int stride)
{
	int						filesize;
	static unsigned char	fileheader[14] = {
		'B', 'M',
		0, 0, 0, 0,
		0, 0, 0, 0,
		54, 0, 0, 0,
	};

	filesize = 14 + 40 + (stride * h);
	fileheader[2] = (unsigned char)(filesize);
	fileheader[3] = (unsigned char)(filesize >> 8);
	fileheader[4] = (unsigned char)(filesize >> 16);
	fileheader[5] = (unsigned char)(filesize >> 24);
	return (fileheader);
}

/*
**	BITMAP_INFO_HEADER contents:
**	First 4 bytes represent the size of the header itself.
**	Following 4 bytes indicate width of the final image.
**	Following 4 bytes indicate height of the final image.
**	Following two bytes will have 1. (check documentation)
**	Following two bytes represent the BPP of the final image.
**	Rest of the values are defaulted to 0.
*/

static unsigned char	*cr_bmap_infoheader(t_fdf *fdf, int h, int w)
{
	static unsigned char	infoheader[40] = {
		40, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		1, 0,
		0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
	};

	infoheader[4] = (unsigned char)(w);
	infoheader[5] = (unsigned char)(w >> 8);
	infoheader[6] = (unsigned char)(w >> 16);
	infoheader[7] = (unsigned char)(w >> 24);
	infoheader[8] = (unsigned char)(h);
	infoheader[9] = (unsigned char)(h >> 8);
	infoheader[10] = (unsigned char)(h >> 16);
	infoheader[11] = (unsigned char)(h >> 24);
	infoheader[14] = (unsigned char)(fdf->img.bpp);
	return (infoheader);
}

/*
**	Writes BMP specific metadata and pixel data into the created file.
**	(WIN_HEIGHT) is given as its negative for INFOHEADER to instruct image
**	to be read as a 'top-down' bitmap.
**
**	In BMP format, each line is padded to nearest 4-byte boundary.
**	Paddingsize is calculated to add 0-3 null bytes after each written line.
*/

void					create_bmp(t_fdf *fdf, char *f)
{
	static unsigned char	padding[3] = {0, 0, 0};
	int						paddingsize;
	int						stride;
	int						fd;
	int						i;

	paddingsize = (4 - (WIN_WIDTH * fdf->img.bpp / 8) % 4) % 4;
	stride = (WIN_WIDTH * fdf->img.bpp / 8) + paddingsize;
	if ((fd = open(f, O_APPEND | O_TRUNC | O_CREAT | O_WRONLY, S_IRWXU)) < 0)
		ft_getout(strerror(errno));
	write(fd, cr_bmap_fileheader(WIN_HEIGHT, stride), 14);
	write(fd, cr_bmap_infoheader(fdf, -(WIN_HEIGHT), WIN_WIDTH), 40);
	i = 0;
	while (i < WIN_HEIGHT)
	{
		write(fd, (unsigned char*)fdf->img.addr + (i * (WIN_WIDTH *
		fdf->img.bpp / 8)), (fdf->img.bpp / 8) * WIN_WIDTH);
		write(fd, padding, paddingsize);
		i++;
	}
	if (close(fd))
		ft_getout(strerror(errno));
}
