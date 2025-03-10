typedef struct Wlwin Wlwin;
typedef struct Clipboard Clipboard;

/* The contents of the clipboard
 * are not stored in the compositor.
 * Instead we signal that we have content
 * and the compositor gives us a pipe
 * to the program that wants it when
 * the content is pasted. */
struct Clipboard {
	QLock lk;
	char *content;

	/* Wayland requires that in order
	 * to put data in to the clipboard
	 * you must be the focused application.
	 * So we must provide the serial we get
	 * on keyboard.enter. */
	u32int serial;

	/* Because we dont actually cough
	 * up the buffer until someone else
	 * asks, we can change the contents
	 * locally without a round trip.
	 * Posted stores if we already made
	 * our round trip */
	int posted;
	int primsel_posted;
};

struct Mouse {
	Point xy;
	int buttons;
	ulong msec;
};

struct Wlwin {
	int dx;
	int dy;
	int monx;
	int mony;
	Mouse mouse;
	Clipboard clip;
	Rectangle r;
	int dirty;

	/* Wayland State */
	int runing;
	int poolsize;
	int pointerserial;
	void *shm_data;
	struct wl_compositor *compositor;
	struct wl_display *display;
	struct wl_surface *surface;
	struct wl_surface *cursorsurface;
	struct xdg_wm_base *xdg_wm_base;
	struct xdg_toplevel *xdg_toplevel;
	struct wl_shm_pool *pool;
	struct wl_buffer *screenbuffer;
	struct wl_buffer *cursorbuffer;
	struct wl_shm *shm;
	struct wl_seat *seat;
	struct wl_data_device_manager *data_device_manager;
	struct wl_data_device *data_device;
	struct wl_pointer *pointer;
	/* Keyboard state */
	struct xkb_state *xkb_state;
	struct xkb_context *xkb_context;

	struct zxdg_decoration_manager_v1 *decoman;

	struct zwp_primary_selection_device_manager_v1 *primsel;
	struct zwp_primary_selection_device_v1 *primsel_device;
};

void wlallocbuffer(Wlwin*);
void wlsetcb(Wlwin*);
void wlsettitle(Wlwin*, char*);
char* wlgetsnarf(Wlwin*);
void wlsetsnarf(Wlwin*, char*);
void wldrawcursor(Wlwin*, Cursorinfo*);
void wlresize(Wlwin*, int, int);
void wlflush(Wlwin*);
