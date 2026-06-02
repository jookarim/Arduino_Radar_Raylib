#include <raylib.h>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <chrono>

const float OFFSET = 80.f;
const int SMALLEST_CIRCLE_SIZE = 150;
const int DISTANCE_BETWEEN_CIRCLES = 180;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1070;
const int LINES_OFFSET = 50;
const float MAX_RADIUS = 950.f;
const float ANGLE_INCREASE_FOR_LINES = 30;
const int TEXT_OFFSET = 20;
Color pointColor = GREEN;
unsigned long long prevTime = 0.f;
const unsigned long long delayTime = 200'000'000;

const Color colors[] =
{
	LIGHTGRAY,
	GRAY,
	YELLOW,
	GOLD,
	ORANGE,
	PINK,
	RED,
	MAROON,
	GREEN,
	LIME,
	DARKGREEN,
	SKYBLUE,
	BLUE,
	PURPLE,
	VIOLET,
	BEIGE,
	BROWN,
	WHITE,
	MAGENTA,
	RAYWHITE
};

void drawSemicircle(float radius, float halfScreenWidth, float screenHeight)
{
	DrawLine(0, screenHeight - OFFSET, halfScreenWidth * 2, screenHeight - OFFSET, GREEN);

	for (float theta = 0.f; theta <= 180.f; theta += 0.01f)
	{
		float x = halfScreenWidth + radius * std::cos(theta * (PI / 180.0f));
		float y = (screenHeight - OFFSET) - (radius * std::sin(theta * (PI / 180.0f)));

		DrawPixel(x, y, GREEN);
	}
}

void drawLines()
{
	for (int i = 1; i <= 5; ++i)
	{
		float xOrigin = SCREEN_WIDTH / 2;
		float xEnd = xOrigin + MAX_RADIUS * std::cos(ANGLE_INCREASE_FOR_LINES * i * (PI / 180.0f));
		float yOrigin = SCREEN_HEIGHT - OFFSET;
		float yEnd  = yOrigin - MAX_RADIUS * std::sin(ANGLE_INCREASE_FOR_LINES * i * (PI / 180.0f));

		DrawLine(xOrigin, yOrigin, xEnd, yEnd, GREEN);
	}
}

float currAngle = 0.f;
int direction = 1;

void drawRadarLines()
{
	for (uint8_t i = 0; i < 200; ++i)
	{
		float xOrigin = (SCREEN_WIDTH / 2 + i * 0.02);
		float yOrigin = SCREEN_HEIGHT - OFFSET;
		float xEnd = xOrigin + MAX_RADIUS * std::cos((currAngle + 0.1 * i) * (PI / 180));
		float yEnd = yOrigin - MAX_RADIUS * std::sin((currAngle + 0.1 * i) * (PI / 180));
		
		uint8_t alpha = 255 - i;

		DrawLine(xOrigin, yOrigin, xEnd, yEnd, Color{0, 255, 0, alpha});

		if (currAngle <= 0.f)
		{
			direction = 1;
		}

		else if (currAngle + i * 0.1 >= 180)
		{
			direction = -1;
		}

		
	}

	currAngle += 10 * GetFrameTime() * direction;
	std::cout << currAngle << "\n";

}

void drawAngles()
{
	for (int i = 1; i <= 5; ++i)
	{
		float xOrigin = SCREEN_WIDTH / 2.0f;
		float yOrigin = SCREEN_HEIGHT - OFFSET;

		float xText = xOrigin + (MAX_RADIUS + TEXT_OFFSET) * std::cos(30 * i * (PI / 180.f));
		float yText = yOrigin - (MAX_RADIUS + TEXT_OFFSET) * std::sin(30 * i * (PI / 180.f));

		float angle = 30.0f * i;
		
		float rot = 90.f - angle;

		const char* text = TextFormat("%d\xC2\xB0", 30 * i);
		float textWidth = (float)MeasureText(text, 12);

		DrawTextPro(
			GetFontDefault(),
			text,
			{ xText, yText},
			{ textWidth / 2.0f, 6.0f }, 
			rot,
			12,
			3,
			GREEN
		);
	}
}

void drawPointObstacle(float distance, float angle)
{
	const unsigned long long currTime = std::chrono::steady_clock::now().time_since_epoch().count();

	float pointX = SCREEN_WIDTH / 2 + (distance / 10.f) * DISTANCE_BETWEEN_CIRCLES * std::cos(angle * (PI / 180));
	float pointY = SCREEN_HEIGHT - OFFSET - (distance / 10.f) * DISTANCE_BETWEEN_CIRCLES * std::sin(angle * (PI / 180));

	const int colorsArrSize = sizeof(colors) / sizeof(Color);
	if (currTime - prevTime >= delayTime)
	{
		pointColor = colors[GetRandomValue(0, colorsArrSize - 1)];
		prevTime = currTime;
	}

	DrawCircle(pointX, pointY, 4, pointColor);
}

void render()
{
	for (int i = 1; i <= 5; ++i)
	{
		drawSemicircle(i * DISTANCE_BETWEEN_CIRCLES, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	}

	drawAngles();

	drawRadarLines();

	drawPointObstacle(22.f, 90.f);
	drawLines();
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window");

	SetConfigFlags(FLAG_MSAA_4X_HINT);

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);


		render();

		EndDrawing();
	}

	CloseWindow();
}