#pragma once
#include "olcPixelGameEngine.h"
#include "clay.h"

#include <chrono>
#include <thread>

void HandleClayErrors(Clay_ErrorData errorData)
{
	std::cout << errorData.errorText.chars << "\n";
} 

static inline Clay_Dimensions MeasureClayText(Clay_StringSlice text, Clay_TextElementConfig *config, void* userData)
{
	return Clay_Dimensions{
		.width = float(text.length * (config->fontSize + config->letterSpacing)), // works because the default PGE font is monospaced
		.height = float(config->fontSize)
	};
}

class ClayImage
{
public:
	ClayImage()
	{
		origImage = new olc::Sprite(0, 0);
		bufferedImage = new olc::Sprite(0, 0);
	}

	ClayImage(const std::string& name)
	{
		origImage = new olc::Sprite(name);
		bufferedImage = new olc::Sprite(0, 0);
	}

public:
	bool UpdateImageDimensions(const int newWidth, const int newHeight)
	{
		if (newWidth != bufferedImage->width || newHeight != bufferedImage->height)
		{
			delete bufferedImage;
			bufferedImage = new olc::Sprite(newWidth, newHeight);
			SampleForBuffer();
			return true;
		}
		return false;
	}

public:
	void SetSprite(const std::string& name)
	{
		delete origImage;
		origImage = new olc::Sprite(name);
		SampleForBuffer();
	}

	olc::Sprite* GetSprite()
	{
		return bufferedImage;
	}

	float GetAspectRatio()
	{
		return (float)origImage->width / (float)origImage->height;
	}
	
private:
	olc::Sprite* origImage; // The original image provided by the user, which is left unchanged
	olc::Sprite* bufferedImage = nullptr; // The modified image, which may have different dimensions. This ultimately gets drawn

	void SampleForBuffer()
	{
		int width = bufferedImage->width;
		int height = bufferedImage->height;
		for(int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
				bufferedImage->SetPixel(x, y, origImage->Sample(x / (float)width, y / (float)height));
	}

public:
	~ClayImage()
	{
		delete origImage;
		delete bufferedImage;
	}
};

class PGEClayRenderer
{
public:
	PGEClayRenderer() { }

	void InitializeClay(olc::PixelGameEngine* pge)
	{
		this->pge = pge;

		uint64_t totalMemorySize = Clay_MinMemorySize();
		buf = new unsigned char[totalMemorySize];
		Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, buf);

		Clay_Initialize(arena, Clay_Dimensions{ (float)pge->ScreenWidth(), (float)pge->ScreenHeight() }, Clay_ErrorHandler{ HandleClayErrors });

		Clay_SetMeasureTextFunction(MeasureClayText, nullptr);
	}

	~PGEClayRenderer()
	{
		delete[] buf;
	}

private:
	olc::PixelGameEngine* pge;

private:
	unsigned char* buf;
	Clay_Arena arena;

public:
	inline olc::Pixel ClayColorToPGE(const Clay_Color& cc) const
	{
		olc::Pixel pgec;
		pgec.r = (uint8_t)cc.r;
		pgec.g = (uint8_t)cc.g;
		pgec.b = (uint8_t)cc.b;
		pgec.a = (uint8_t)cc.a;
		return pgec;
	}

private:
	olc::Pixel GetColorAndSetMode(const Clay_Color& cc) const
	{
		olc::Pixel pgec = ClayColorToPGE(cc);
		if (pgec.a != 255)
			pge->SetPixelMode(olc::Pixel::ALPHA);
		return pgec;
	}

	void DrawSpriteRounded(int x, int y, olc::Sprite* sprite, int r)
	{ }

private:
	std::string debugCommandNames[8] = {"none", "rectangle", "border", "text", "image", "scissor start", "scissor end", "custom"};

public:
	void UpdateInput(float fElapsedTime)
	{
        Clay_SetLayoutDimensions(Clay_Dimensions{ (float)pge->ScreenWidth(), (float)pge->ScreenHeight() });
		Clay_SetPointerState(Clay_Vector2{ (float)pge->GetMouseX(), (float)pge->GetMouseY() }, pge->GetMouse(0).bHeld);
		int shiftHeld = pge->GetKey(olc::Key::SHIFT).bHeld;
		Clay_UpdateScrollContainers(true, Clay_Vector2{ shiftHeld*(float)pge->GetMouseWheel()/30, (1-shiftHeld)*(float)pge->GetMouseWheel()/30 }, fElapsedTime);
	}

	void RenderLayout(Clay_RenderCommandArray renderCommands)
	{
		olc::Pixel::Mode stashedMode = pge->GetPixelMode();

		bool debug = pge->GetKey(olc::Key::D).bHeld;

		if (debug) std::cout << "\nFRAME:\n";

		for (int i = 0; i < renderCommands.length; i++) {
			Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];
			int x = renderCommand->boundingBox.x;
			int y = renderCommand->boundingBox.y;
			int w = renderCommand->boundingBox.width;
			int h = renderCommand->boundingBox.height;

			pge->SetPixelMode(olc::Pixel::NORMAL);

			switch (renderCommand->commandType)
			{
				case CLAY_RENDER_COMMAND_TYPE_NONE:
				{
					break;
				}

				case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: 
				{
					olc::Pixel c = GetColorAndSetMode(renderCommand->renderData.rectangle.backgroundColor);
					if (c.a == 0) break;
					float r = renderCommand->renderData.rectangle.cornerRadius.topLeft;

					if (debug)
						std::cout << "Color: " << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a << "\n";

					if(r == 0.0f)
						pge->FillRect(x,y,w,h,c);
					else
						pge->FillRectRounded(x, y, w, h, r, c);
					break;
				}

				case CLAY_RENDER_COMMAND_TYPE_BORDER:
				{
					olc::Pixel c = GetColorAndSetMode(renderCommand->renderData.border.color);
					if (c.a == 0) break;

					int rTL = renderCommand->renderData.border.cornerRadius.topLeft;
					int rTR = renderCommand->renderData.border.cornerRadius.topRight;
					int rBR = renderCommand->renderData.border.cornerRadius.bottomRight;
					int rBL = renderCommand->renderData.border.cornerRadius.bottomLeft;

					int wT = renderCommand->renderData.border.width.top;
					int wR = renderCommand->renderData.border.width.right;
					int wB = renderCommand->renderData.border.width.bottom;
					int wL = renderCommand->renderData.border.width.left;

					int rTRclamped = std::max(rTR, wT); // These ensure the vertical borders don't overlap
					int rBRclamped = std::max(rBR, wB); // with the horizontal borders if the radius is
					int rBLclamped = std::max(rBL, wB); // very small and the border widths are very big
					int rTLclamped = std::max(rTL, wT);

					int TLoverlap = (rTL < wT); // I fucking hate off-by-one bugs
					int BRoverlap = (rBR < wB);

					pge->FillRect(x + rTL + TLoverlap, y,                                w - rTL - rTR - 1 - TLoverlap, wT, c); //Top border
					pge->FillRect(x + rBL + 1,         y + h - wB,                       w - rBL - rBR - 1 - BRoverlap, wB, c); //Bottom border
					pge->FillRect(x,                   y + rTLclamped + 1 - TLoverlap,   wL,                            h - rTLclamped - rBLclamped + TLoverlap - 1, c); //Left border
					pge->FillRect(x + w - wR,          y + rTRclamped,                   wR,                            h - rTRclamped - rBRclamped - 1 + BRoverlap, c); //Right border
				
					if (rTR > 0) pge->FillRing(x + w - rTR - 1, y + rTR,         rTR, wT, c, 0x03);
					if (rBR > 0) pge->FillRing(x + w - rBR - 1, y + h - rBR - 1, rBR, wB, c, 0x0C);
					if (rBL > 0) pge->FillRing(x + rBL,         y + h - rBL - 1, rBL, wB, c, 0x30);
					if (rTL > 0) pge->FillRing(x + rTL,         y + rTL,         rTL, wT, c, 0xC0);

					if (rTR < wT) pge->FillRect(x + w - rTR - 1, y + rTR + 1, rTR + 1, wT - rTR - 1, c);
					if (rBR < wB) pge->FillRect(x + w - rBR - 1, y + h - wB,  rBR + 1, wB - rBR - 1, c);
					if (rBL < wB) pge->FillRect(x,               y + h - wB,  rBL + 1, wB - rBL - 1, c);
					if (rTL < wT) pge->FillRect(x,               y + rTL + 1, rTL + 1, wT - rTL - 1, c);

					if (debug)
					{
						std::cout << "\tWidths       : (wT, wR, wB, wL) " << wT << " " << wR << " " << wB << " " << wL << "\n";
					}

					break;
				}

				case CLAY_RENDER_COMMAND_TYPE_TEXT:
				{
					olc::Pixel c = GetColorAndSetMode(renderCommand->renderData.text.textColor);
					if (c.a == 0) break;

					uint32_t scale = renderCommand->renderData.text.fontSize / 8;

					std::string text = renderCommand->renderData.text.stringContents.chars;
					int number = renderCommand->renderData.text.stringContents.length;
					int spacing = renderCommand->renderData.text.letterSpacing;
					pge->DrawString(x, y, text.substr(0,number), c, scale, spacing);
					if (debug)
					{
						std::cout << "\tChars: " << renderCommand->renderData.text.stringContents.chars << "\n";
						std::cout << "\tBase chars length: " << std::string(renderCommand->renderData.text.stringContents.baseChars).length() << "\n";
					}
					break;
				}

				case CLAY_RENDER_COMMAND_TYPE_IMAGE:
				{
					ClayImage* image = (ClayImage*)renderCommand->renderData.image.imageData;

					image->UpdateImageDimensions(w, h);

					pge->DrawSprite(x, y, image->GetSprite());

					break;
				}

				case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
				{
					pge->SetScreenClipRect(x, y, w, h);
					if (debug)
					{
						std::cout << "\tclip horizontal: " << renderCommand->renderData.clip.horizontal << "\n";
						std::cout << "\tclip vertical: " << renderCommand->renderData.clip.vertical << "\n";
					}
					break;
				}

				case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
				{
					pge->SetScreenClipRect(0, 0, pge->ScreenWidth(), pge->ScreenHeight());
					break;
				}

				default:
					std::cout << "Unhandled render type: " << debugCommandNames[(int)renderCommand->commandType] << "\n";
			}

			if (debug)
				std::cout << "\tDid command " << renderCommand->id << " of type " << debugCommandNames[(int)renderCommand->commandType]
				<< "  (" << x << ", " << y << ") (" << w << ", " << h << ")" 
				<< "\n\n";
		}

		pge->SetPixelMode(stashedMode);
	}
};
