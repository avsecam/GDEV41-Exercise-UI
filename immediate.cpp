#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Struct to encapsulate our UI library
struct UiLibrary {
  // ID of the currently hot widget
  int hot = -1;

  // ID of the currently active widget
  int active = -1;

  // Creates a button with the specified text and bounds
  // Returns true if this button was clicked in this frame
  bool Button(int id, const std::string& text, const Rectangle& bounds) {
    bool result = false;
    Color bgColor = GRAY;
    Color color = BLACK;

    // If this button is the currently active widget, that means
    // the user is currently interacting with this widget
    if (id == active) {
      color = SKYBLUE;
      bgColor = DARKGREEN;
      // If the user released the mouse button while we are active,
      // register as a click
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        // Make sure that we are still the hot widget
        // (User might have dragged mouse to another widget, in which case we
        // will not count as a button click)
        if (id == hot) {
          result = true;
          bgColor = GREEN;
        }

        // Set ourselves to not be active anymore
        active = -1;
      }
    }

    // If we are currently the hot widget
    if (id == hot) {
      if (id != active) {
        bgColor = GREEN;
      }
      // If the user pressed the left mouse button, that means the user started
      // interacting with this widget, so we set this widget as active
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        active = id;
      }
    }

    // If the mouse cursor is hovering within our boundaries
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
      // Set this widget to be the hot widget
      hot = id;
    }
    // If the mouse cursor is not on top of this widget, and this widget
    // was previously the hot widget, set the hot widget to -1
    // We check if this widget was the previously hot widget since there is a
    // possibility that the hot widget is now a different widget, and we don't
    // want to overwrite that with -1
    else if (hot == id) {
      hot = -1;
      bgColor = GRAY;
    }

    // Draw our button regardless of what happens
    DrawRectangleRec(bounds, bgColor);
    DrawText(text.c_str(), bounds.x, bounds.y, 14, color);

    return result;
  }

	void Label(const std::string& text, const Rectangle& bounds) {
		DrawText(text.c_str(), bounds.x, bounds.y, 14, PURPLE);
	}

  bool Checkbox(int id, const bool& isChecked, const std::string& textOn, const std::string& textOff, const Rectangle& bounds) {
		bool result = isChecked;

		Color bgColorOff = ORANGE;
    Color bgColorOn = PINK;

    Color bgColor = (result) ? bgColorOn : bgColorOff;
    Color color = BLACK;

    if (id == active) {
      color = SKYBLUE;
      bgColor = DARKGREEN;
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (id == hot) {
          result = !isChecked;
          bgColor = (result) ? bgColorOn : bgColorOff;
        }

        active = -1;
      }
    }

    if (id == hot) {
      if (id != active) {
        bgColor = GREEN;
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        active = id;
      }
    }

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
      hot = id;
    } else if (hot == id) {
      hot = -1;
      bgColor = (isChecked) ? bgColorOn : bgColorOff;
    }

    DrawRectangleRec(bounds, bgColor);
    DrawText((isChecked) ? textOn.c_str() : textOff.c_str(), bounds.x, bounds.y, 14, color);

    return result;
  }
};

int main() {
  int windowWidth = 800, windowHeight = 600;
  InitWindow(windowWidth, windowHeight, "Immediate Mode");
  SetTargetFPS(60);

  UiLibrary uiLibrary;

	bool canResize = false;

  while (!WindowShouldClose()) {
    ClearBackground(WHITE);
    BeginDrawing();
		canResize = uiLibrary.Checkbox(10, canResize, "Resolution resizable.", "Resolution locked.", {10, 60, 80, 40});

    if (uiLibrary.Button(0, "800x600", {10, 10, 80, 40}) && canResize) {
			SetWindowSize(800, 600);
    }
    if (uiLibrary.Button(1, "1000x600", {100, 10, 80, 40}) && canResize) {
			SetWindowSize(1000, 600);
    }
    if (uiLibrary.Button(2, "1200x600", {190, 10, 80, 40}) && canResize) {
			SetWindowSize(1200, 600);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
