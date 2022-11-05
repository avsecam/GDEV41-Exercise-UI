#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <string>
#include <vector>

// Generic UI component
struct UIComponent {
  // Rectangle reprsenting the bounds of the UI component
  Rectangle bounds;

  // Draws this particular UI component
  // Set as abstract so that child widgets will implement this for us
  virtual void Draw() = 0;

  // Handles a mouse click event
  // Set as abstract so that child widgets will implement this for us
  // Returns a boolean indicating whether this UI component successfully handled
  // the event
  virtual bool HandleClick(Vector2 clickPosition) = 0;
};

// Generic UI component that can contain other UI components as children
struct UIContainer : public UIComponent {
  std::vector<UIComponent*> children;

  // Adds a child to the container
  void AddChild(UIComponent* child) { children.push_back(child); }

  // Draw
  void Draw() override {
    // Since we are just a container for other widgets, we simply
    // call the draw function of other widgets.
    // This results in a pre-order traversal when we also draw child widgets
    // that are also containers
    for (size_t i = 0; i < children.size(); ++i) {
      children[i]->Draw();
    }
  }

  // Handles a mouse click event
  // Returns a boolean indicating whether this UI component successfully handled
  // the event
  bool HandleClick(Vector2 clickPosition) override {
    // Since we are just a container for other widgets, we call the HandleClick
    // function of our child widgets Since later children are drawn last, we do
    // the opposite of draw where we start from the last child. This results in
    // a pre-order traversal but in the reverse order.
    for (size_t i = children.size(); i > 0; --i) {
      // If a child already handles the click event, we instantly return so no
      // more child widgets handle the click
      if (children[i - 1]->HandleClick(clickPosition)) {
        return true;
      }
    }

    return false;
  }
};

// Button widget
struct Button : public UIComponent {
  // Text displayed by the button
  std::string text;
  void (*functionOnClick)();

  // Draw
  void Draw() override {
    DrawRectangleRec(bounds, GRAY);
    DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);
  }

  // Handle mouse click
  // Returns a boolean indicating whether this UI component successfully handled
  // the event
  bool HandleClick(Vector2 clickPosition) override {
    // Check if the mouse click position is within our bounds
    if (CheckCollisionPointRec(clickPosition, bounds)) {
      functionOnClick();
      return true;
    }

    return false;
  }
};

// Checkbox widget
struct Checkbox : public UIComponent {
  std::string text;
  bool isChecked = false;

  void Draw() override {
		Color color = (isChecked) ? BLUE : RED;
		DrawRectangleRec(bounds, color);
		if (hasBeenInteractedWith) {
			std::string newText = (isChecked) ? "Checkbox ON" : "Checkbox OFF";
			DrawText(newText.c_str(), bounds.x, bounds.y, 14, BLACK);
		} else {
			DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);
		}
	}

	bool HandleClick(Vector2 clickPosition) override {
		// Check if the mouse click position is within our bounds
    if (CheckCollisionPointRec(clickPosition, bounds)) {
			if (!hasBeenInteractedWith) {
				hasBeenInteractedWith = true;
			}
      isChecked = !isChecked;
      return true;
    }

    return false;
	}
	
	private:
		bool hasBeenInteractedWith = false;
};

// Text display widget
struct Label : public UIComponent {
  // Text to be displayed
  std::string text;

  // Draw
  void Draw() override {
    DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);
  }

  // Handle mouse click
  // Returns a boolean indicating whether this UI component successfully handled
  // the event
  bool HandleClick(Vector2 clickPosition) override {
    // Always return false since we're not going to handle click events for this
    // particular widget (unless you have to)
    return false;
  }
};

// Struct to encapsulate our UI library
struct UILibrary {
  // Root container
  UIContainer rootContainer;

  // Updates the current UI state
  void Update() {
    // If the left mouse button was released, we handle the click from the root
    // container
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      rootContainer.HandleClick(GetMousePosition());
    }
  }

  // Draw
  void Draw() { rootContainer.Draw(); }
};

void sampleFunctionOnClick1() { std::cout << "sample 1\n"; }

void sampleFunctionOnClick2() { std::cout << "sample 2\n"; }

int main() {
  int windowWidth = 800, windowHeight = 600;
  InitWindow(windowWidth, windowHeight, "Retained Mode");
  SetTargetFPS(60);

  UILibrary uiLibrary;
  uiLibrary.rootContainer.bounds = {10, 10, 600, 500};

  Button button;
  button.text = "Hello!";
  button.bounds = {120, 10, 80, 40};
  button.functionOnClick = sampleFunctionOnClick1;
  uiLibrary.rootContainer.AddChild(&button);

  Button button2;
  button2.text = "Hi!";
  button2.bounds = {210, 10, 80, 40};
  button2.functionOnClick = sampleFunctionOnClick2;
  uiLibrary.rootContainer.AddChild(&button2);

  Label label;
  label.text = "This is a label";
  label.bounds = {10, 20, 100, 40};
  uiLibrary.rootContainer.AddChild(&label);

	Checkbox checkbox;
	checkbox.text = "I am a checkbox :o";
	checkbox.bounds = {10, 60, 100, 40};
	uiLibrary.rootContainer.AddChild(&checkbox);

  while (!WindowShouldClose()) {
    uiLibrary.Update();

    ClearBackground(WHITE);
    BeginDrawing();
    uiLibrary.Draw();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
