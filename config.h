/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;  /* border pixel of windows */
static const unsigned int snap      = 32; /* snap pixel */
static const int swallowfloating    = 0;  /* 1 means swallow floating windows by default */
static int enablegaps               = 0;  /* enables gaps, used by togglegaps */
static const unsigned int gapp      = 15; /* gap between windows */
static const unsigned int gapinc    = 10; /* outer gap between windows and screen edge */
static const int smartgaps          = 0;  /* no outer gap, untill there are 'smargaps' open windows */
static const int showbar            = 0;  /* 0 means no bar */
static const int topbar             = 1;  /* 0 means bottom bar */

static const char *fonts[]          = { "monospace:size=12:antialias=true:autohint=true" };
static const char col_normbg[]      = "#222222";
static const char col_normfg[]      = "#bbbbbb";
static const char col_selfg[]       = "#eeeeee";
static const char col_selbg[]       = "#005577";
static const char *colors[][3]      = {
	/*               fg          bg          border   */
	[SchemeNorm] = { col_normfg, col_normbg, "#444444" },
	[SchemeSel]  = { col_selfg,  col_selbg,  col_selbg },
};

static const char autostart[] = "$HOME/.dwmstart";

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance  title tags mask iscentered isfloating isterminal noswallow monitor */
	{ "st",       NULL,     NULL, 0,        0,         0,         1,        -1,       -1 },
	{ "floating", NULL,     NULL, 0,        1,         1,         0,         0,       -1 }
};

static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "vanitygaps.c"
static const Layout layouts[] = {
	{ "[]=",  tile },
	{ "TTT",  bstack },
	{ "[M]",  monocle },
	{ "H[]",  deck },
	{ "HH ",  grid },
	{ "|M|",  centeredmaster },
	{ "HHH",  gaplessgrid },
	{ "###",  horizgrid },
	{ "[@]",  spiral },
	{ "[\\]", dwindle },
	{ "><>",  NULL },
	{ ">M>",  centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define MODKEY_MR Mod3Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char layoutmenu_cmd[] = "dwm-layoutmenu";

static Key keys[] = {
	/* applications */
	{ MODKEY,           XK_r, spawn,         {.v = dmenucmd } },
	{ MODKEY,           XK_s, togglescratch, {.v = scratchpadcmd } },

	{ MODKEY,           XK_Return, spawn, SHCMD("dwm-samedir") },
	{ MODKEY|ShiftMask, XK_Return, spawn, SHCMD("$TERMINAL") },
	{ MODKEY,           XK_u,      spawn, SHCMD("dwm-samedir -c floating") },
	{ MODKEY|ShiftMask, XK_u,      spawn, SHCMD("$TERMINAL -c floating") },

	{ MODKEY,           XK_w, spawn, SHCMD("$BROWSER") },
	{ MODKEY,           XK_m, spawn, SHCMD("$CLIP") },
	{ MODKEY,           XK_x, spawn, SHCMD("$LOCK") },
	{ MODKEY|ShiftMask, XK_x, spawn, SHCMD("xkill") },

	/* layouts */
	{ MODKEY,                       XK_a, layoutmenu, {0} },
	{ MODKEY|ControlMask,           XK_t, setlayout, {.v = &layouts[0]} },  /* tile */
	{ MODKEY|ControlMask|ShiftMask, XK_t, setlayout, {.v = &layouts[1]} },  /* bottomtile  */
	{ MODKEY|ControlMask,           XK_m, setlayout, {.v = &layouts[2]} },  /* monocle */
	{ MODKEY|ControlMask|ShiftMask, XK_m, setlayout, {.v = &layouts[3]} },  /* deck */
	{ MODKEY|ControlMask,           XK_u, setlayout, {.v = &layouts[4]} },  /* grid */
	{ MODKEY|ControlMask|ShiftMask, XK_u, setlayout, {.v = &layouts[5]} },  /* centeredmaster */
	{ MODKEY|ControlMask,           XK_i, setlayout, {.v = &layouts[6]} },  /* verticalgrid */
	{ MODKEY|ControlMask,           XK_i, setlayout, {.v = &layouts[7]} },  /* horizontalgrid */
	{ MODKEY|ControlMask,           XK_o, setlayout, {.v = &layouts[8]} },  /* spiral */
	{ MODKEY|ControlMask|ShiftMask, XK_o, setlayout, {.v = &layouts[9]} },  /* dwindle */
	{ MODKEY|ControlMask,           XK_p, setlayout, {.v = &layouts[10]} }, /* floating */
	{ MODKEY|ControlMask|ShiftMask, XK_p, setlayout, {.v = &layouts[11]} }, /* centeredfloatingmaster */

	/* window manager */
	{ MODKEY,           XK_b,       togglebar,      {0} },
	{ MODKEY,           XK_space,   zoom,           {0} },
	{ MODKEY|ShiftMask, XK_space,   togglefloating, {0} },
	{ MODKEY,           XK_Tab,     view,           {0} },
	{ MODKEY,           XK_q,       killclient,     {0} },
	{ MODKEY|ShiftMask, XK_q,       quit,           {0} },

	/* resize */
	{ MODKEY_MR,           XK_j, moveresize, {.v = (int[]){0, 25,0,0 }} },
	{ MODKEY_MR,           XK_k, moveresize, {.v = (int[]){0,-25,0,0 }} },
	{ MODKEY_MR,           XK_l, moveresize, {.v = (int[]){ 25,0,0,0 }} },
	{ MODKEY_MR,           XK_h, moveresize, {.v = (int[]){-25,0,0,0 }} },
	{ MODKEY_MR|ShiftMask, XK_j, moveresize, {.v = (int[]){0,0,0, 25 }} },
	{ MODKEY_MR|ShiftMask, XK_k, moveresize, {.v = (int[]){0,0,0,-25 }} },
	{ MODKEY_MR|ShiftMask, XK_l, moveresize, {.v = (int[]){0,0, 25,0 }} },
	{ MODKEY_MR|ShiftMask, XK_h, moveresize, {.v = (int[]){0,0,-25,0 }} },
	{ MODKEY_MR|ControlMask,           XK_j, moveresizeedge, {.v = "b"} },
	{ MODKEY_MR|ControlMask,           XK_k, moveresizeedge, {.v = "t"} },
	{ MODKEY_MR|ControlMask,           XK_h, moveresizeedge, {.v = "l"} },
	{ MODKEY_MR|ControlMask,           XK_l, moveresizeedge, {.v = "r"} },
	{ MODKEY_MR|ControlMask|ShiftMask, XK_j, moveresizeedge, {.v = "B"} },
	{ MODKEY_MR|ControlMask|ShiftMask, XK_k, moveresizeedge, {.v = "T"} },
	{ MODKEY_MR|ControlMask|ShiftMask, XK_h, moveresizeedge, {.v = "L"} },
	{ MODKEY_MR|ControlMask|ShiftMask, XK_l, moveresizeedge, {.v = "R"} },

	{ MODKEY,           XK_KP_Down,  moveresize, {.v = (int[]){0, 25,0,0 }} },
	{ MODKEY,           XK_KP_Up,    moveresize, {.v = (int[]){0,-25,0,0 }} },
	{ MODKEY,           XK_KP_Right, moveresize, {.v = (int[]){ 25,0,0,0 }} },
	{ MODKEY,           XK_KP_Left,  moveresize, {.v = (int[]){-25,0,0,0 }} },
	{ MODKEY|ShiftMask, XK_KP_Down,  moveresize, {.v = (int[]){0,0,0, 25 }} },
	{ MODKEY|ShiftMask, XK_KP_Up,    moveresize, {.v = (int[]){0,0,0,-25 }} },
	{ MODKEY|ShiftMask, XK_KP_Right, moveresize, {.v = (int[]){0,0, 25,0 }} },
	{ MODKEY|ShiftMask, XK_KP_Left,  moveresize, {.v = (int[]){0,0,-25,0 }} },
	{ MODKEY|ControlMask,           XK_KP_Down,  moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_KP_Up,    moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_KP_Left,  moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_KP_Right, moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_KP_Down,  moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_KP_Up,    moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_KP_Left,  moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_KP_Right, moveresizeedge, {.v = "R"} },

	/* focus */
	{ MODKEY,           XK_j, focusstack, {.i = +1 } },
	{ MODKEY,           XK_k, focusstack, {.i = -1 } },
	{ MODKEY|ShiftMask, XK_j, pushstack,  {.i = +1 } },
	{ MODKEY|ShiftMask, XK_k, pushstack,  {.i = -1 } },
	{ MODKEY,           XK_i, incnmaster, {.i = +1 } },
	{ MODKEY,           XK_d, incnmaster, {.i = -1 } },
	{ MODKEY,           XK_h, setmfact,   {.f = -0.05} },
	{ MODKEY,           XK_l, setmfact,   {.f = +0.05} },

	/* gap */
	{ MODKEY,             XK_bracketleft,  incgaps,     {.i = +gapinc } },
	{ MODKEY,             XK_bracketright, incgaps,     {.i = -gapinc } },
	{ MODKEY|ShiftMask,   XK_g,            defaultgaps, {0} },
	{ MODKEY,             XK_g,            togglegaps,  {0} },

	/* multi monitor */
	{ MODKEY,           XK_comma,  focusmon, {.i = -1 } },
	{ MODKEY,           XK_period, focusmon, {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,   {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,   {.i = +1 } },

	/* tags */
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

