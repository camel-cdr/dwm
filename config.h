/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;  /* border pixel of windows */
static const unsigned int snap      = 32; /* snap pixel */
static const int swallowfloating    = 0;  /* 1 means swallow floating windows by default */
static int enablegaps               = 0;  /* enables gaps, used by togglegaps */
static const unsigned int gapp      = 15; /* gap between windows */
static const unsigned int gapinc    = 5;  /* outer gap between windows and screen edge */
static const int smartgaps          = 0;  /* no outer gap, untill there are 'smargaps' open windows */
static const int showbar            = 1;  /* 0 means no bar */
static const int topbar             = 1;  /* 0 means bottom bar */

static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_normbg[]      = "#222222";
static const char col_normfg[]      = "#bbbbbb";
static const char col_selfg[]       = "#eeeeee";
static const char col_selbg[]       = "#005577";
static const char *colors[][3]      = {
	/*               fg          bg          border   */
	[SchemeNorm] = { col_normfg, col_normbg, "#444444" },
	[SchemeSel]  = { col_selfg,  col_selbg,  col_selbg },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance  title tags mask iscentered isfloating isterminal noswallow monitor */
	{ "Gimp",     NULL,     NULL, 0,        0,         1,         0,         0,       -1 },
	{ "st",       NULL,     NULL, 0,        0,         0,         1,        -1,       -1 },
	{ "floating", NULL,     NULL, 0,        1,         1,         0,         0,       -1 }
};

static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

/* layout(s) */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",  tile },                   /* Default: Master on left, slaves on right */
	{ "TTT",  bstack },                 /* Master on top, slaves on bottom */

	{ "[@]",  spiral },                 /* Fibonacci spiral */
	{ "[\\]", dwindle },                /* Decreasing in size right and leftward */


	{ "H[]",  deck },                   /* Master on left, slaves in monocle-like mode on right */
	{ "[M]",  monocle },                /* All windows on top of eachother */

	{ "HHH",  gaplessgrid },
	{ "|M|",  centeredmaster },         /* Master in middle, slaves on sides */

	{ "><>",  NULL },                   /* no layout function means floating behavior */
	{ ">M>",  centeredfloatingmaster }, /* Same but master floats */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_normbg, "-nf", col_normfg, "-sb", col_selbg, "-sf", col_selfg, NULL };

static Key keys[] = {
	/* applications */
	{ MODKEY,           XK_r,      spawn, {.v = dmenucmd } },
	{ MODKEY,           XK_Return, spawn, SHCMD("dwm-samedir") },
	{ MODKEY|ShiftMask, XK_Return, spawn, SHCMD("$TERMINAL") },
	{ MODKEY,           XK_u,      spawn, SHCMD("dwm-samedir -c floating") },
	{ MODKEY|ShiftMask, XK_u,      spawn, SHCMD("$TERMINAL -c floating") },

	{ MODKEY,           XK_w, spawn, SHCMD("$BROWSER") },
	{ MODKEY,           XK_m, spawn, SHCMD("$CLIP") },
	{ MODKEY,           XK_x, spawn, SHCMD("$LOCK") },
	{ MODKEY|ShiftMask, XK_x, spawn, SHCMD("xkill") },

	/* layouts */
	{ MODKEY|ControlMask,           XK_t, setlayout, {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ControlMask,           XK_b, setlayout, {.v = &layouts[1]} }, /* bstack */
	{ MODKEY|ControlMask,           XK_o, setlayout, {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ControlMask|ShiftMask, XK_o, setlayout, {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY|ControlMask|ShiftMask, XK_m, setlayout, {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ControlMask,           XK_m, setlayout, {.v = &layouts[5]} }, /* monocle */
	{ MODKEY|ControlMask,           XK_i, setlayout, {.v = &layouts[6]} }, /* gaplessgrid */
	{ MODKEY|ControlMask|ShiftMask, XK_i, setlayout, {.v = &layouts[7]} }, /* centeredmaster */
	{ MODKEY|ControlMask,           XK_p, setlayout, {.v = &layouts[8]} }, /* centeredfloatingmaster */
	{ MODKEY|ControlMask|ShiftMask, XK_p, setlayout, {.v = &layouts[9]} }, /* centeredfloatingmaster */

	/* window manager */
	{ MODKEY,           XK_b,     togglebar,      {0} },
	{ MODKEY,           XK_space, zoom,           {0} },
	{ MODKEY|ShiftMask, XK_space, togglefloating, {0} },
	{ MODKEY,           XK_Tab,   view,           {0} },
	{ MODKEY,           XK_q,     killclient,     {0} },
	{ MODKEY|ShiftMask, XK_q,     quit,           {0} },

	{ MODKEY,           XK_j, focusstack, {.i = +1 } },
	{ MODKEY,           XK_k, focusstack, {.i = -1 } },
	{ MODKEY|ShiftMask, XK_j, pushstack,  {.i = +1 } },
	{ MODKEY|ShiftMask, XK_k, pushstack,  {.i = -1 } },
	{ MODKEY,           XK_i, incnmaster, {.i = +1 } },
	{ MODKEY,           XK_d, incnmaster, {.i = -1 } },
	{ MODKEY,           XK_h, setmfact,   {.f = -0.05} },
	{ MODKEY,           XK_l, setmfact,   {.f = +0.05} },

	{ MODKEY,             XK_bracketleft,  incgaps,     {.i = +gapinc } },
	{ MODKEY,             XK_bracketright, incgaps,     {.i = -gapinc } },
	{ MODKEY|ShiftMask,   XK_g,            defaultgaps, {0} },
	{ MODKEY,             XK_g,            togglegaps,  {0} },

	{ MODKEY,           XK_comma,  focusmon, {.i = -1 } },
	{ MODKEY,           XK_period, focusmon, {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,   {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,   {.i = +1 } },

	{ MODKEY,                 XK_0, view, {.ui = ~0 } },
	{ MODKEY|ShiftMask,       XK_0, tag,  {.ui = ~0 } },
	TAGKEYS(            XK_1, 0)
	TAGKEYS(            XK_2, 1)
	TAGKEYS(            XK_3, 2)
	TAGKEYS(            XK_4, 3)
	TAGKEYS(            XK_5, 4)
	TAGKEYS(            XK_6, 5)
	TAGKEYS(            XK_7, 6)
	TAGKEYS(            XK_8, 7)
	TAGKEYS(            XK_9, 8)

#if 0
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
#endif
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click        event mask button   function        argument */
	{ ClkWinTitle,  0,         Button2, zoom,           {0} },
	{ ClkClientWin, MODKEY,    Button1, movemouse,      {0} },
	{ ClkClientWin, MODKEY,    Button2, togglefloating, {0} },
	{ ClkClientWin, MODKEY,    Button3, resizemouse,    {0} },
	{ ClkTagBar,    0,         Button1, view,           {0} },
	{ ClkTagBar,    0,         Button3, toggleview,     {0} },
	{ ClkTagBar,    MODKEY,    Button1, tag,            {0} },
	{ ClkTagBar,    MODKEY,    Button3, toggletag,      {0} },
};

