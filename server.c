#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>

#define CANVAS_SIZE 100
#define PIXEL_SIZE 5

typedef struct
{
    int r, g, b;
} Pixel;

Pixel canvas[CANVAS_SIZE][CANVAS_SIZE];
struct lws *clients[100];
int client_count = 0;

static int callback_canvas(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        printf("Client connected\n");
        clients[client_count++] = wsi;
        for (int x = 0; x < CANVAS_SIZE; x++)
        {
            for (int y = 0; y < CANVAS_SIZE; y++)
            {
                Pixel color = canvas[x][y];
                if (color.r != 0 || color.g != 0 || color.b != 0)
                {
                    char buffer[128];
                    snprintf(buffer, sizeof(buffer), "%d %d %d %d %d", x, y, color.r, color.g, color.b);
                    lws_write(wsi, (unsigned char *)buffer, strlen(buffer), LWS_WRITE_TEXT);
                }
            }
        }
        break;

    case LWS_CALLBACK_RECEIVE:
    {
        int x, y, r, g, b;
        sscanf((char *)in, "%d %d %d %d %d", &x, &y, &r, &g, &b);
        canvas[x][y] = (Pixel){r, g, b};
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%d %d %d %d %d", x, y, r, g, b);
        for (int i = 0; i < client_count; i++)
        {
            if (clients[i] != wsi)
            {
                lws_write(clients[i], (unsigned char *)buffer, strlen(buffer), LWS_WRITE_TEXT);
            }
        }
        break;
    }

    case LWS_CALLBACK_CLOSED:
        printf("Client disconnected\n");
        for (int i = 0; i < client_count; i++)
        {
            if (clients[i] == wsi)
            {
                clients[i] = clients[--client_count];
                break;
            }
        }
        break;

    default:
        break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "canvas-protocol",
        callback_canvas,
        0,
        128,
    },
    {NULL, NULL, 0, 0}};

int main()
{
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    info.port = 8080;
    info.iface = NULL; // 모든 네트워크 인터페이스에서 수신

    info.protocols = protocols;
    struct lws_context *context = lws_create_context(&info);
    if (!context)
    {
        fprintf(stderr, "Failed to create WebSocket context\n");
        return -1;
    }

    printf("WebSocket server started on ws://your_public_ip:8080\n");
    while (1)
    {
        lws_service(context, 0);
    }

    lws_context_destroy(context);
    return 0;
}
