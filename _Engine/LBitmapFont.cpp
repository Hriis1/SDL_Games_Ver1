#include "LBitmapFont.h"

LBitmapFont::LBitmapFont()
{
}

LBitmapFont::~LBitmapFont()
{
	free();
}

bool LBitmapFont::buildFont(const std::string& path, SDL_Window* window, SDL_Renderer* renderer)
{
    //Get rid of preexisting texture
    free();

    //Load bitmap image
    if (!_fontTexture.loadPixelsFromFile(path,window))
    {
        printf("Unable to load bitmap font surface!\n");
        return false;
    }

    //Get the background color
    Uint32 bgColor = _fontTexture.getPixel32(0, 0);

    //Set the cell dimensions
    int cellW = _fontTexture.getWidth() / 16;
    int cellH = _fontTexture.getHeight() / 16;

    //New line variables
    int top = cellH;
    int baseA = cellH;

    //The current character we're setting
    int currentChar = 0;

	//Go through the cell rows
	for (int rows = 0; rows < 16; ++rows)
	{
		//Go through the cell columns
		for (int cols = 0; cols < 16; ++cols)
		{
			//Set the character offset
			_chars[currentChar].x = cellW * cols;
			_chars[currentChar].y = cellH * rows;

			//Set the dimensions of the character
			_chars[currentChar].w = cellW;
			_chars[currentChar].h = cellH;

			//Find Left Side
			//Go through pixel columns
			for (int pCol = 0; pCol < cellW; ++pCol)
			{
				//Go through pixel rows
				for (int pRow = 0; pRow < cellH; ++pRow)
				{
					//Get the pixel offsets
					int pX = (cellW * cols) + pCol;
					int pY = (cellH * rows) + pRow;

					//If a non colorkey pixel is found
					if (_fontTexture.getPixel32(pX, pY) != bgColor)
					{
						//Set the x offset
						_chars[currentChar].x = pX;

						//Break the loops
						pCol = cellW;
						pRow = cellH;
					}
				}
			}

			//Find Right Side
			//Go through pixel columns
			for (int pColW = cellW - 1; pColW >= 0; --pColW)
			{
				//Go through pixel rows
				for (int pRowW = 0; pRowW < cellH; ++pRowW)
				{
					//Get the pixel offsets
					int pX = (cellW * cols) + pColW;
					int pY = (cellH * rows) + pRowW;

					//If a non colorkey pixel is found
					if (_fontTexture.getPixel32(pX, pY) != bgColor)
					{
						//Set the width
						_chars[currentChar].w = (pX - _chars[currentChar].x) + 1;

						//Break the loops
						pColW = -1;
						pRowW = cellH;
					}
				}
			}

			//Find Top
			//Go through pixel rows
			for (int pRow = 0; pRow < cellH; ++pRow)
			{
				//Go through pixel columns
				for (int pCol = 0; pCol < cellW; ++pCol)
				{
					//Get the pixel offsets
					int pX = (cellW * cols) + pCol;
					int pY = (cellH * rows) + pRow;

					//If a non colorkey pixel is found
					if (_fontTexture.getPixel32(pX, pY) != bgColor)
					{
						//If new top is found
						if (pRow < top)
						{
							top = pRow;
						}

						//Break the loops
						pCol = cellW;
						pRow = cellH;
					}
				}
			}

			//Find Bottom of A
			if (currentChar == 'A')
			{
				//Go through pixel rows
				for (int pRow = cellH - 1; pRow >= 0; --pRow)
				{
					//Go through pixel columns
					for (int pCol = 0; pCol < cellW; ++pCol)
					{
						//Get the pixel offsets
						int pX = (cellW * cols) + pCol;
						int pY = (cellH * rows) + pRow;

						//If a non colorkey pixel is found
						if (_fontTexture.getPixel32(pX, pY) != bgColor)
						{
							//Bottom of a is found
							baseA = pRow;

							//Break the loops
							pCol = cellW;
							pRow = -1;
						}
					}
				}
			}

			//Go to the next character
			++currentChar;
		}
	}

	//Calculate space
	_space = cellW / 2;

	//Calculate new line
	_newLine = baseA - top;

	//Lop off excess top pixels
	for (int i = 0; i < 256; ++i)
	{
		_chars[i].y += top;
		_chars[i].h -= top;
	}

	//Create final texture
	_fontTexture.initRenderer(renderer);
	if (!_fontTexture.loadFromPixels())
	{
		printf("Unable to create font texture!\n");
		return false;
	}

	return true;
}

void LBitmapFont::free()
{
	_fontTexture.free();
}

void LBitmapFont::renderText(int x, int y, const std::string& text)
{
	//If the font has been built
	if (_fontTexture.getWidth() > 0)
	{
		//Temp offsets
		int curX = x, curY = y;

		//Go through the text
		for (int i = 0; i < text.length(); ++i)
		{
			//If the current character is a space
			if (text[i] == ' ')
			{
				//Move over
				curX += _space;
			}
			//If the current character is a newline
			else if (text[i] == '\n')
			{
				//Move down
				curY += _newLine;

				//Move back
				curX = x;
			}
			else
			{
				//Get the ASCII value of the character
				int ascii = (unsigned char)text[i];

				//Show the character
				_fontTexture.render(curX, curY, &_chars[ascii]);

				//Move over the width of the character with one pixel of padding
				curX += _chars[ascii].w + 1;
			}
		}
	}
}
