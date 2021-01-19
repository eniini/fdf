/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 17:22:30 by eniini            #+#    #+#             */
/*   Updated: 2020/10/26 21:23:06 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# ifdef __MACH__

#  define KEY_A 0x00
#  define KEY_S 0x01
#  define KEY_D 0x02
#  define KEY_F 0x03
#  define KEY_H 0x04
#  define KEY_G 0x05
#  define KEY_Z 0x06
#  define KEY_X 0x07
#  define KEY_C 0x08
#  define KEY_V 0x09
#  define KEY_B 0x0B
#  define KEY_Q 0x0C
#  define KEY_W 0x0D
#  define KEY_E 0x0E
#  define KEY_R 0x0F
#  define KEY_Y 0x10
#  define KEY_T 0x11
#  define KEY_O 0x1F
#  define KEY_U 0x20
#  define KEY_I 0x22
#  define KEY_P 0x23
#  define KEY_L 0x25
#  define KEY_J 0x26
#  define KEY_K 0x28
#  define KEY_N 0x2D
#  define KEY_M 0x2E
#  define KEY_1 0x12
#  define KEY_2 0x13
#  define KEY_3 0x14
#  define KEY_4 0x15
#  define KEY_6 0x16
#  define KEY_5 0x17
#  define KEY_9 0x19
#  define KEY_7 0x1A
#  define KEY_8 0x1C
#  define KEY_0 0x1D
#  define KEY_SPACE 0x31
#  define KEY_ESC 0x35

# elif __unix__

#  define KEY_A 0x61
#  define KEY_B 0x62
#  define KEY_C 0x63
#  define KEY_D 0x64
#  define KEY_E 0x65
#  define KEY_F 0x66
#  define KEY_G 0x67
#  define KEY_H 0x68
#  define KEY_I 0x69
#  define KEY_J 0x6A
#  define KEY_K 0x6B
#  define KEY_L 0x6C
#  define KEY_M 0x6D
#  define KEY_N 0x6E
#  define KEY_O 0x6F
#  define KEY_P 0x70
#  define KEY_Q 0x71
#  define KEY_R 0x72
#  define KEY_S 0x73
#  define KEY_T 0x74
#  define KEY_U 0x75
#  define KEY_V 0x76
#  define KEY_W 0x77
#  define KEY_X 0x78
#  define KEY_Y 0x79
#  define KEY_Z 0x7A
#  define KEY_0 0x30
#  define KEY_1 0x31
#  define KEY_2 0x32
#  define KEY_3 0x33
#  define KEY_4 0x34
#  define KEY_5 0x35
#  define KEY_6 0x36
#  define KEY_7 0x37
#  define KEY_8 0x38
#  define KEY_9 0x39
#  define KEY_SPACE 0x20
#  define KEY_ESC 0xFF1B

# endif

#endif
