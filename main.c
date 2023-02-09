#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <ViGEm/Client.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_GAMECONTROLLER);

  SDL_GameController *controller = SDL_GameControllerOpen(0);

  SDL_Log("%s", SDL_GameControllerMapping(controller));

  PVIGEM_CLIENT vigem_client = vigem_alloc();
  PVIGEM_TARGET xbox_target = vigem_target_x360_alloc();
  vigem_connect(vigem_client);
  vigem_target_add(vigem_client, xbox_target);

  XUSB_REPORT report;

  int running = 1;
  int send = 0;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
        case SDL_QUIT:
          running = 0;
          break;

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:

          switch (ev.cbutton.button) {
            case SDL_CONTROLLER_BUTTON_A:
              report.wButtons = XUSB_GAMEPAD_A;
              break;
            case SDL_CONTROLLER_BUTTON_B:
              report.wButtons = XUSB_GAMEPAD_B;
              break;
            case SDL_CONTROLLER_BUTTON_X:
              report.wButtons = XUSB_GAMEPAD_X;
              break;
            case SDL_CONTROLLER_BUTTON_Y:
              report.wButtons = XUSB_GAMEPAD_Y;
              break;

          }

          send = 1;
          break;
      }
    }

    if (send) {
      vigem_target_x360_update(vigem_client, xbox_target, report);
      send = 0;
    }
  };

  SDL_Quit();

  vigem_target_remove(vigem_client, xbox_target);
  vigem_disconnect(vigem_client);
  vigem_target_free(xbox_target);
  vigem_free(vigem_client);

  return 0;
}