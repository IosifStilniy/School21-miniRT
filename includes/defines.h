/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:21:04 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 19:23:44 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# ifndef KEY_Q
#  define KEY_Q 12
# endif

# ifndef KEY_E
#  define KEY_E 14
# endif

# ifndef KEY_A
#  define KEY_A 0
# endif

# ifndef KEY_S
#  define KEY_S 1
# endif

# ifndef KEY_D
#  define KEY_D 2
# endif

# ifndef KEY_W
#  define KEY_W 13
# endif

# ifndef KEY_L
#  define KEY_L 37
# endif

# ifndef KEY_C
#  define KEY_C 8
# endif

# ifndef KEY_R
#  define KEY_R 15
# endif

# ifndef KEY_I
#  define KEY_I 34
# endif

# ifndef KEY_N
#  define KEY_N 45
# endif

# ifndef KEY_UP
#  define KEY_UP 126
# endif

# ifndef KEY_LEFT
#  define KEY_LEFT 123
# endif

# ifndef KEY_DOWN
#  define KEY_DOWN 125
# endif

# ifndef KEY_RIGHT
#  define KEY_RIGHT 124
# endif

# ifndef KEY_ESC
#  define KEY_ESC 53
# endif

# ifndef KEY_CMND
#  define KEY_CMND 259
# endif

# ifndef KEY_SHIFT
#  define KEY_SHIFT 257
# endif

# ifndef KEY_SPACE
#  define KEY_SPACE 49
# endif

# ifndef KEY_CNTRL
#  define KEY_CNTRL 256
# endif

# ifndef KEY_CLBRCT
#  define KEY_CLBRCT 30
# endif

# ifndef KEY_OPBRCT
#  define KEY_OPBRCT 33
# endif

# ifndef LMB
#  define LMB 1
# endif

# ifndef MMB
#  define MMB 3
# endif

# ifndef SCRL_UP
#  define SCRL_UP 5
# endif

# ifndef SCRL_DOWN
#  define SCRL_DOWN 4
# endif

# ifndef X_EVENT_KEY_EXIT
#  define X_EVENT_KEY_EXIT 17
# endif

# ifndef X_EVENT_KEY_PRESS
#  define X_EVENT_KEY_PRESS 2
# endif

# ifndef X_EVENT_MOUSE_PRESS
#  define X_EVENT_MOUSE_PRESS 4
# endif

# ifndef ESC_TEXT
#  define ESC_TEXT "\033[92mClosing MiniRT.\033[0m"
# endif

# ifndef ERR_TEXT
#  define ERR_TEXT "Error"
# endif

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

# ifndef RESX
#  define RESX 1920
# endif

# ifndef RESY
#  define RESY 1080
# endif

# ifndef SHIFT_SPEED
#  define SHIFT_SPEED 5
# endif

# ifndef DEFANG
#  define DEFANG 2.f
# endif

# ifndef FRAMECLR
#  define FRAMECLR 0x00FFFFFF
# endif

# ifndef NORMCLR
#  define NORMCLR 0x00FF0000
# endif

# ifndef CRNRS
#  define CRNRS 5
# endif

# ifndef GRIDSIZE
#  define GRIDSIZE 20
# endif

# ifndef GRIDLINES
#  define GRIDLINES 6
# endif

# ifndef PRMTVS
#  define PRMTVS "spplcycnhp"
# endif

# ifndef NUMPRMTVS
#  define NUMPRMTVS 5
# endif

# ifndef RNDSGMNTS
#  define RNDSGMNTS 24
# endif

# ifndef CAMLEGEND
#  define CAMLEGEND "sources/interface/caminterface.xpm"
# endif

# ifndef OBJLEGEND
#  define OBJLEGEND "sources/interface/objinterface.xpm"
# endif

# ifndef INVINP
#  define INVINP "needs one arg: [filename].rt"
# endif

# ifndef INVFILE
#  define INVFILE "Error"
# endif

# ifndef DUPDET
#  define DUPDET "duplicate elements"
# endif

# ifndef INVDEF
#  define INVDEF "invalid definition of scene parameters"
# endif

# ifndef INVNUM
#  define INVNUM "invalid num type: float for coordinates and int for color"
# endif

# ifndef INVCRD
#  define INVCRD "incorrect definition of orientation in space"
# endif

# ifndef SPACES
#  define SPACES " \t\v\f\r"
# endif

# ifndef NUMSPACES
#  define NUMSPACES "-+0123456789. \t\v\f\r"
# endif

# ifndef WORLD
#  define WORLD NULL
# endif

# ifndef THREADCOUNT
#  define THREADCOUNT 16
# endif

#endif /* DEFINES_H */