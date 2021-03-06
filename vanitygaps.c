/* Key binding functions */
static void defaultgaps(const Arg *arg);
static void togglegaps(const Arg *arg);

/* Layouts */
static void bstack(Monitor *m);
static void centeredmaster(Monitor *m);
static void centeredfloatingmaster(Monitor *m);
static void deck(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void dwindle(Monitor *m);
static void spiral(Monitor *m);
static void gaplessgrid(Monitor *m);
static void grid(Monitor *m);
static void monocle(Monitor *m);
static void tile(Monitor *);

/* Key binding functions */
static void defaultgaps(const Arg *arg);
static void incgaps(const Arg *arg);
static void togglegaps(const Arg *arg);

/* Layouts */
static void bstack(Monitor *m);
static void centeredmaster(Monitor *m);
static void centeredfloatingmaster(Monitor *m);
static void deck(Monitor *m);
static void dwindle(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void gaplessgrid(Monitor *m);
static void grid(Monitor *m);
static void horizgrid(Monitor *m);
static void spiral(Monitor *m);
static void tile(Monitor *m);

/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc, float *mf, float *sf);


static void
togglegaps(const Arg *arg)
{
	enablegaps = !enablegaps;
	arrange(NULL);
}

static void
defaultgaps(const Arg *arg)
{
	selmon->gapp = (gapp < 0 ? 0 : gapp);
	arrange(selmon);
}

static void
incgaps(const Arg *arg)
{
	int g = selmon->gapp + arg->i;
	selmon->gapp = (g < 0 ? 0 : g);
	arrange(selmon);
}


static void
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc, float *mf, float *sf)
{
	unsigned int n, oe = enablegaps, ie = enablegaps;
	float mfacts = 0, sfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (m->nmaster && n < m->nmaster)
			++mfacts;
		else
			++sfacts;
	}
	if (smartgaps && n == 1) {
		oe = 0; // outer gaps disabled when only one client
	}

	*oh = m->gapp*oe; // outer horizontal gap
	*ov = m->gapp*oe; // outer vertical gap
	*ih = m->gapp*ie; // inner horizontal gap
	*iv = m->gapp*ie; // inner vertical gap
	*nc = n;            // number of clients
	*mf = mfacts;       // total factor of master area
	*sf = sfacts;       // total factor of stack area
}

/***
 * Layouts
 */

/*
 * Bottomstack layout + gaps + cfacts
 * https://dwm.suckless.org/patches/bottomstack/
 */
static void
bstack(Monitor *m)
{
	unsigned int i, n;
	int mx, my, mh, mw, sx = 0, sy = 0, sh = 0, sw = 0, oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv * (MIN(n, m->nmaster) - 1);

	if (m->nmaster && n > m->nmaster) {
		sh = (mh - ih) * (1 - m->mfact);
		mh = (mh - ih) * m->mfact;
		sx = mx;
		sy = my + mh + ih;
		sw = m->ww - 2*ov - iv * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (1 / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw * (1 / sfacts) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
	}
}

/*
 * Centred master layout + gaps + cfacts
 * https://dwm.suckless.org/patches/centeredmaster/
 */
void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts, lfacts = 0, rfacts = 0;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	mfacts = 0;
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mfacts += 1; // total factor of master area
		else if ((n - m->nmaster) % 2)
			lfacts += 1; // total factor of left hand stacke area
		else
			rfacts += 1; // total factor of right hand stack area
	}

	/* initialize areas */
	mx = m->wx + ov;
	my = m->wy + oh;
	mh = m->wh - 2*oh - ih * ((!m->nmaster ? n : MIN(n, m->nmaster)) - 1);
	mw = m->ww - 2*ov;
	lh = m->wh - 2*oh - ih * (((n - m->nmaster) / 2) - 1);
	rh = m->wh - 2*oh - ih * (((n - m->nmaster) / 2) - ((n - m->nmaster) % 2 ? 0 : 1));

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (n - m->nmaster > 1) {
			/* ||<-S->|<---M--->|<-S->|| */
			mw = (m->ww - 2*ov - 2*iv) * m->mfact;
			lw = (m->ww - mw - 2*ov - 2*iv) / 2;
			mx += lw + iv;
		} else {
			/* ||<---M--->|<-S->|| */
			mw = (mw - iv) * m->mfact;
			lw = m->ww - mw - iv - 2*ov;
		}
		rw = lw;
		lx = m->wx + ov;
		ly = m->wy + oh;
		rx = mx + mw + iv;
		ry = m->wy + oh;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			resize(c, mx, my, mw - (2*c->bw), mh * (1 / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				resize(c, lx, ly, lw - (2*c->bw), lh * (1 / lfacts) - (2*c->bw), 0);
				ly += HEIGHT(c) + ih;
			} else {
				resize(c, rx, ry, rw - (2*c->bw), rh * (1 / rfacts) - (2*c->bw), 0);
				ry += HEIGHT(c) + ih;
			}
		}
	}
}

void
centeredfloatingmaster(Monitor *m)
{
	unsigned int i, n;
	float mivf, mfacts, sfacts;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	mivf = 0.8; // master inner vertical gap factor

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv*(n - 1);

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->ww * m->mfact - iv*mivf*(MIN(n, m->nmaster) - 1);
			mh = m->wh * 0.9;
		} else {
			mw = m->ww * 0.9 - iv*mivf*(MIN(n, m->nmaster) - 1);
			mh = m->wh * m->mfact;
		}
		mx = m->wx + (m->ww - mw) / 2;
		my = m->wy + (m->wh - mh - 2*oh) / 2;

		sx = m->wx + ov;
		sy = m->wy + oh;
		sh = m->wh - 2*oh;
		sw = m->ww - 2*ov - iv*(n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			/* nmaster clients are stacked horizontally, in the center of the screen */
			resize(c, mx, my, mw * (1 / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv*mivf;
			focus(c);
		} else {
			/* stack clients are stacked horizontally */
			resize(c, sx, sy, sw * (1 / sfacts) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
	}
	restack(m);
}

/*
 * Deck layout + gaps + cfacts
 * https://dwm.suckless.org/patches/deck/
 */
static void
deck(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sy = my;
		sh = m->wh - 2*oh;
	}

	if (n - m->nmaster > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "D %d", n - m->nmaster);

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw - (2*c->bw), mh * (1 / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh - (2*c->bw), 0);
		}
}

/*
 * Fibonacci layout + gaps
 * https://dwm.suckless.org/patches/fibonacci/
 */
static void
fibonacci(Monitor *m, int s)
{
	unsigned int i, n;
	int nx, ny, nw, nh;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	nx = m->wx + ov;
	ny = oh;
	nw = m->ww - 2*ov;
	nh = m->wh - 2*oh;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2*c->bw)
		   || (!(i % 2) && nw / 2 > 2*c->bw)) {
			if (i < n - 1) {
				if (i % 2)
					nh = (nh - ih) / 2;
				else
					nw = (nw - iv) / 2;

				if ((i % 4) == 2 && !s)
					nx += nw + iv;
				else if ((i % 4) == 3 && !s)
					ny += nh + ih;
			}
			if ((i % 4) == 0) {
				if (s)
					ny += nh + ih;
				else
					ny -= nh + ih;
			}
			else if ((i % 4) == 1)
				nx += nw + iv;
			else if ((i % 4) == 2)
				ny += nh + ih;
			else if ((i % 4) == 3) {
				if (s)
					nx += nw + iv;
				else
					nx -= nw + iv;
			}
			if (i == 0)	{
				if (n != 1)
					nw = (m->ww - 2*ov - iv) * m->mfact;
				ny = m->wy + oh;
			}
			else if (i == 1)
				nw = m->ww - nw - iv - 2*ov;
			i++;
		}

		resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), False);
	}
}

static void
dwindle(Monitor *m)
{
	fibonacci(m, 1);
}

static void
spiral(Monitor *m)
{
	fibonacci(m, 0);
}

/*
 * Gappless grid layout + gaps (ironically)
 * https://dwm.suckless.org/patches/gaplessgrid/
 */
void
gaplessgrid(Monitor *m)
{
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);
	if (n == 0)
		return;

	/* grid dimensions */
	for (cols = 0; cols <= n/2; cols++)
		if (cols*cols >= n)
			break;
	if (n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	/* window geometries */
	cw = cols ? (m->ww - 2*ov - iv*(cols - 1)) / cols : m->ww - 2*ov;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for (i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if (i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? (m->wh - 2*oh - ih*(rows - 1)) / rows : m->wh - 2*oh;
		cx = m->wx + ov + cn*(cw + iv);
		cy = m->wy + oh + rn*(ch + ih);
		resize(c, cx, cy, cw - 2*c->bw, ch - 2*c->bw, False);
		rn++;
		if (rn >= rows) {
			rn = 0;
			cn++;
		}
	}
}

/*
 * Gridmode layout + gaps
 * https://dwm.suckless.org/patches/gridmode/
 */
static void
grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, cols, rows;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	/* grid dimensions */
	for (rows = 0; rows <= n/2; rows++)
		if (rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (m->wh - 2*oh - ih * (rows - 1)) / (rows ? rows : 1);
	cw = (m->ww - 2*ov - iv * (cols - 1)) / (cols ? cols : 1);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * (cw + iv) + ov;
		cy = m->wy + (i % rows) * (ch + ih) + oh;
		resize(c, cx, cy, cw - 2*c->bw, ch - 2*c->bw, False);
		i++;
	}
}

/*
 * Horizontal grid layout + gaps
 * https://dwm.suckless.org/patches/horizgrid/
 */
void
horizgrid(Monitor *m) {
	Client *c;
	unsigned int n, i;
	int w = 0, oh, ov, ih, iv;
	int ntop, nbottom = 0;
	float mfacts, sfacts;

	/* Count windows */
	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	if (n == 1) { /* Just fill the whole screen */
		c = nexttiled(m->clients);
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2*ov - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
	} else if (n == 2) { /* Split vertically */
		w = (m->ww - 2*ov - iv) / 2;
		c = nexttiled(m->clients);
		resize(c, m->wx + ov, m->wy + oh, w - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
		c = nexttiled(c->next);
		resize(c, m->wx + ov + w + iv, m->wy + oh, w - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
	} else {
		ntop = n / 2;
		nbottom = n - ntop;
		for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if (i < ntop)
				resize(
					c,
					m->wx + ov + i * ((m->ww - 2*ov - iv*(ntop - 1)) / ntop + iv),
					m->wy + oh,
					(m->ww - 2*ov - iv*(ntop - 1)) / ntop - (2*c->bw),
					(m->wh - 2*oh - ih) / 2 - (2*c->bw),
					False
				);
			else
				resize(
					c,
					m->wx + ov + (i - ntop) * ((m->ww - 2*ov - iv*(nbottom - 1)) / nbottom + iv),
					m->wy + oh + ih + (m->wh - 2*oh - ih) / 2,
					(m->ww - 2*ov - iv*(nbottom - 1)) / nbottom - (2*c->bw),
					(m->wh - 2*oh - ih) / 2 - (2*c->bw),
					False
				);
		}
	}
}


static void
monocle(Monitor *m)
{
	unsigned int n;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2 * c->bw - 2 * ov, m->wh - 2 * c->bw - 2 * oh, 0);
}

static void
tile(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sy = my;
		sh = m->wh - 2*oh - ih * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw - (2*c->bw), mh * (1 / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (1 / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
}

