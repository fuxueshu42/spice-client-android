/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2010 Red Hat, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __SPICE_CLIENT_CHANNEL_H__
#define __SPICE_CLIENT_CHANNEL_H__

G_BEGIN_DECLS

#include "spice-types.h"
#include "spice-glib-enums.h"
#include "spice-util.h"

#define SPICE_TYPE_CHANNEL            (spice_channel_get_type ())
#define SPICE_CHANNEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SPICE_TYPE_CHANNEL, SpiceChannel))
#define SPICE_CHANNEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SPICE_TYPE_CHANNEL, SpiceChannelClass))
#define SPICE_IS_CHANNEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPICE_TYPE_CHANNEL))
#define SPICE_IS_CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SPICE_TYPE_CHANNEL))
#define SPICE_CHANNEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SPICE_TYPE_CHANNEL, SpiceChannelClass))

typedef struct spice_msg_in  spice_msg_in;
typedef struct spice_msg_out spice_msg_out;

/**
 * SpiceChannelEvent:
 *
 * @SPICE_CHANNEL_NONE: no event, or ignored event
 * @SPICE_CHANNEL_OPENED: connection is authentified and ready
 * @SPICE_CHANNEL_CLOSED: connection is closed normally (sent if channel was ready)
 * @SPICE_CHANNEL_ERROR_CONNECT: connection error
 * @SPICE_CHANNEL_ERROR_TLS: SSL error
 * @SPICE_CHANNEL_ERROR_LINK: error during link process
 * @SPICE_CHANNEL_ERROR_AUTH: authentication error
 * @SPICE_CHANNEL_ERROR_IO: IO error
 *
 * An event, emitted by #SpiceChannel::channel-event signal.
 **/
typedef enum
{
    SPICE_CHANNEL_NONE = 0,
    SPICE_CHANNEL_OPENED = 10,
    SPICE_CHANNEL_SWITCHING,
    SPICE_CHANNEL_CLOSED,
    SPICE_CHANNEL_ERROR_CONNECT = 20,
    SPICE_CHANNEL_ERROR_TLS,
    SPICE_CHANNEL_ERROR_LINK,
    SPICE_CHANNEL_ERROR_AUTH,
    SPICE_CHANNEL_ERROR_IO,
} SpiceChannelEvent;

struct _SpiceChannel
{
    GObject parent;
    spice_channel *priv;
    /* Do not add fields to this struct */
};

struct _SpiceChannelClass
{
    GObjectClass parent_class;

    /*< private >*/
    /* virtual methods, coroutine context */
    void (*handle_msg)(SpiceChannel *channel, spice_msg_in *msg);
    void (*channel_up)(SpiceChannel *channel);
    void (*iterate_write)(SpiceChannel *channel);
    void (*iterate_read)(SpiceChannel *channel);

    /*< public >*/
    /* signals, main context */
    void (*channel_event)(SpiceChannel *channel, SpiceChannelEvent event);
    void (*open_fd)(SpiceChannel *channel, int with_tls);

    /*< private >*/
    /* virtual method, any context */
    void (*channel_disconnect)(SpiceChannel *channel);
    /*
     * If adding fields to this struct, remove corresponding
     * amount of padding to avoid changing overall struct size
     */
    gchar _spice_reserved[SPICE_RESERVED_PADDING - 4 * sizeof(void*)];
};

GType spice_channel_get_type(void) G_GNUC_CONST;

typedef void (*spice_msg_handler)(SpiceChannel *channel, spice_msg_in *in);

SpiceChannel *spice_channel_new(SpiceSession *s, int type, int id);
void spice_channel_destroy(SpiceChannel *channel);
gboolean spice_channel_connect(SpiceChannel *channel);
gboolean spice_channel_open_fd(SpiceChannel *channel, int fd);
void spice_channel_disconnect(SpiceChannel *channel, SpiceChannelEvent event);
gboolean spice_channel_test_capability(SpiceChannel *channel, guint32 cap);
void spice_channel_set_capability(SpiceChannel *channel, guint32 cap);

G_END_DECLS

#endif /* __SPICE_CLIENT_CHANNEL_H__ */
