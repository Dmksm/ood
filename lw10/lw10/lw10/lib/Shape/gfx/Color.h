#pragma once
#include <string>
#include <cstdint>
#include <stdexcept>

class Color
{
public:
    Color(const std::string& color)
    {
        if (!IsValidHexCode(color))
        {
            std::string message = "Undefined hex color code = " + color + ". ";
            throw std::logic_error(message);
        }
        m_color = color;
    };

    uint32_t GetRGBFormat()
    {
        return HexToUint32(m_color);
    }

    std::string GetHexFormat()
    {
        return m_color;
    }

private:
    std::string m_color = "";

    uint32_t HexToUint32(const std::string& hexColor) const
    {
        std::string hexCode = hexColor;
        if (hexCode.at(0) == '#')
        {
            hexCode.erase(0, 1);
        }
        while (hexCode.length() != 6)
        {
            hexCode += "0";
        }
        while (hexCode.length() != 8)
        {
            hexCode += "F";
        }

        return std::stoul(hexCode, nullptr, 16);
    }

    bool IsValidHexCode(const std::string& hexColor) const
    {
        const unsigned AVAILABLE_HEX_SIZE = 7;
        const char FIRST_HEX_SYMBOL = '#';
        if ((hexColor[0] != FIRST_HEX_SYMBOL) || (hexColor.length() != AVAILABLE_HEX_SIZE))
        {
            return false;
        }

        for (int position = 1; position < hexColor.length(); position++)
        {
            if (!((hexColor[position] >= '0' && hexColor[position] <= '9')
                || (hexColor[position] >= 'a' && hexColor[position] <= 'f')
                || (hexColor[position] >= 'A' && hexColor[position] <= 'F')))
            {
                return false;
            }
        }

        return true;
    }
};