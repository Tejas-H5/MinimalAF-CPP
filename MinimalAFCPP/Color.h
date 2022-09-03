#pragma once
#include<glm/glm.hpp>
#include<math.h>

namespace af {
    static inline glm::vec4 rgb255(float r, float g, float b, float a = 1.0f) {
        return rgb(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
    }

    /// <summary>
    /// floats, 0.0f to 1.0f
    /// </summary>
    static inline glm::vec4 rgb(float r, float g, float b, float a = 1.0f)  {
        return glm::vec4(r, g, b, a);
    }

    /// <summary>
    /// Make a color from HSV. Think of that colour circle you often see in drawing prorams,
    /// </summary>
    /// <param name="h">An angle around the color circle in degrees</param>
    /// <param name="s">A distance from the center of the color circle from 0 being the center to 1 being the outside</param>
    /// <param name="v">Darkness, with 0 being dark and 1 being bright</param>
    /// <param name="a"></param>
    /// <returns></returns>
    static inline glm::vec4 hsv(float h, float s, float v, float a = 1.0f) {
        float c = v * s;
        float x = c * (1 - std::abs(h / 60.0f) % 2 - 1);
        float m = v - c;
        float r = 0, g = 0, b = 0;
        if (h < 60) {
            r = c;
            g = x;
        }
        else if (h < 120) {
            r = x;
            g = c;
        }
        else if (h < 180) {
            g = c;
            b = x;
        }
        else if (h < 240) {
            g = x;
            b = c;
        }
        else if (h < 300) {
            r = x;
            b = c;
        }
        else {
            r = c;
            b = x;
        }

        return rgb(r + m, g + m, b + m, a);
    }

    const static glm::vec4 COLOR_ALICE_BLUE = rgb255(240, 248, 255);
    const static glm::vec4 COLOR_ANTIQUE_WHITE = rgb255(250, 235, 215);
    const static glm::vec4 COLOR_AQUA = rgb255(0, 255, 255);
    const static glm::vec4 COLOR_AQUAMARINE = rgb255(127, 255, 212);
    const static glm::vec4 COLOR_AZURE = rgb255(240, 255, 255);
    const static glm::vec4 COLOR_BEIGE = rgb255(245, 245, 220);
    const static glm::vec4 COLOR_BISQUE = rgb255(255, 228, 196);
    const static glm::vec4 COLOR_BLACK = rgb255(0, 0, 0);
    const static glm::vec4 COLOR_BLANCHED_ALMOND = rgb255(255, 235, 205);
    const static glm::vec4 COLOR_BLUE = rgb255(0, 0, 255);
    const static glm::vec4 COLOR_BLUE_VIOLET = rgb255(138, 43, 226);
    const static glm::vec4 COLOR_BROWN = rgb255(165, 42, 42);
    const static glm::vec4 COLOR_BURLY_WOOD = rgb255(222, 184, 135);
    const static glm::vec4 COLOR_CADET_BLUE = rgb255(95, 158, 160);
    const static glm::vec4 COLOR_CHARTREUSE = rgb255(127, 255, 0);
    const static glm::vec4 COLOR_CHOCOLATE = rgb255(210, 105, 30);
    const static glm::vec4 COLOR_CORAL = rgb255(255, 127, 80);
    const static glm::vec4 COLOR_CORNFLOWER_BLUE = rgb255(100, 149, 237);
    const static glm::vec4 COLOR_CORN_SILK = rgb255(255, 248, 220);
    const static glm::vec4 COLOR_CRIMSON = rgb255(220, 20, 60);
    const static glm::vec4 COLOR_CYAN = rgb255(0, 255, 255);
    const static glm::vec4 COLOR_DARK_BLUE = rgb255(0, 0, 139);
    const static glm::vec4 COLOR_DARK_CYAN = rgb255(0, 139, 139);
    const static glm::vec4 COLOR_DARK_GOLDENROD = rgb255(184, 134, 11);
    const static glm::vec4 COLOR_DARK_GRAY = rgb255(169, 169, 169);
    const static glm::vec4 COLOR_DARK_GREEN = rgb255(0, 100, 0);
    const static glm::vec4 COLOR_DARK_GREY = rgb255(169, 169, 169);
    const static glm::vec4 COLOR_DARK_KHAKI = rgb255(189, 183, 107);
    const static glm::vec4 COLOR_DARK_MAGENTA = rgb255(139, 0, 139);
    const static glm::vec4 COLOR_DARK_OLIVE_GREEN = rgb255(85, 107, 47);
    const static glm::vec4 COLOR_DARK_ORANGE = rgb255(255, 140, 0);
    const static glm::vec4 COLOR_DARK_ORCHID = rgb255(153, 50, 204);
    const static glm::vec4 COLOR_DARK_RED = rgb255(139, 0, 0);
    const static glm::vec4 COLOR_DARK_SALMON = rgb255(233, 150, 122);
    const static glm::vec4 COLOR_DARK_SEA_GREEN = rgb255(143, 188, 143);
    const static glm::vec4 COLOR_DARK_SLATE_BLUE = rgb255(72, 61, 139);
    const static glm::vec4 COLOR_DARK_SLATE_GRAY = rgb255(47, 79, 79);
    const static glm::vec4 COLOR_DARK_SLATE_GREY = rgb255(47, 79, 79);
    const static glm::vec4 COLOR_DARK_TURQUOISE = rgb255(0, 206, 209);
    const static glm::vec4 COLOR_DARK_VIOLET = rgb255(148, 0, 211);
    const static glm::vec4 COLOR_DEEP_PINK = rgb255(255, 20, 147);
    const static glm::vec4 COLOR_DEEP_SKY_BLUE = rgb255(0, 191, 255);
    const static glm::vec4 COLOR_DIM_GRAY = rgb255(105, 105, 105);
    const static glm::vec4 COLOR_DIM_GREY = rgb255(105, 105, 105);
    const static glm::vec4 COLOR_DODGER_BLUE = rgb255(30, 144, 255);
    const static glm::vec4 COLOR_FIREBRICK = rgb255(178, 34, 34);
    const static glm::vec4 COLOR_FLORAL_WHITE = rgb255(255, 250, 240);
    const static glm::vec4 COLOR_FOREST_GREEN = rgb255(34, 139, 34);
    const static glm::vec4 COLOR_FUCHSIA = rgb255(255, 0, 255);
    const static glm::vec4 COLOR_GAINSBORO = rgb255(220, 220, 220);
    const static glm::vec4 COLOR_GHOST_WHITE = rgb255(248, 248, 255);
    const static glm::vec4 COLOR_GOLD = rgb255(255, 215, 0);
    const static glm::vec4 COLOR_GOLDENROD = rgb255(218, 165, 32);
    const static glm::vec4 COLOR_GRAY = rgb255(128, 128, 128);
    const static glm::vec4 COLOR_GREEN = rgb255(0, 128, 0);
    const static glm::vec4 COLOR_GREEN_YELLOW = rgb255(173, 255, 47);
    const static glm::vec4 COLOR_GREY = rgb255(128, 128, 128);
    const static glm::vec4 COLOR_HONEYDEW = rgb255(240, 255, 240);
    const static glm::vec4 COLOR_HOT_PINK = rgb255(255, 105, 180);
    const static glm::vec4 COLOR_INDIAN_RED = rgb255(205, 92, 92);
    const static glm::vec4 COLOR_INDIGO = rgb255(75, 0, 130);
    const static glm::vec4 COLOR_IVORY = rgb255(255, 255, 240);
    const static glm::vec4 COLOR_KHAKI = rgb255(240, 230, 140);
    const static glm::vec4 COLOR_LAVENDER = rgb255(230, 230, 250);
    const static glm::vec4 COLOR_LAVENDER_BLUSH = rgb255(255, 240, 245);
    const static glm::vec4 COLOR_LAWN_GREEN = rgb255(124, 252, 0);
    const static glm::vec4 COLOR_LEMON_CHIFFON = rgb255(255, 250, 205);
    const static glm::vec4 COLOR_LIGHT_BLUE = rgb255(173, 216, 230);
    const static glm::vec4 COLOR_LIGHT_CORAL = rgb255(240, 128, 128);
    const static glm::vec4 COLOR_LIGHT_CYAN = rgb255(224, 255, 255);
    const static glm::vec4 COLOR_LIGHT_GOLDENROD_YELLOW = rgb255(250, 250, 210);
    const static glm::vec4 COLOR_LIGHT_GRAY = rgb255(211, 211, 211);
    const static glm::vec4 COLOR_LIGHT_GREEN = rgb255(144, 238, 144);
    const static glm::vec4 COLOR_LIGHT_GREY = rgb255(211, 211, 211);
    const static glm::vec4 COLOR_LIGHT_PINK = rgb255(255, 182, 193);
    const static glm::vec4 COLOR_LIGHT_SALMON = rgb255(255, 160, 122);
    const static glm::vec4 COLOR_LIGHT_SEA_GREEN = rgb255(32, 178, 170);
    const static glm::vec4 COLOR_LIGHT_SKY_BLUE = rgb255(135, 206, 250);
    const static glm::vec4 COLOR_LIGHT_SLATE_GRAY = rgb255(119, 136, 153);
    const static glm::vec4 COLOR_LIGHT_SLATE_GREY = rgb255(119, 136, 153);
    const static glm::vec4 COLOR_LIGHT_STEEL_BLUE = rgb255(176, 196, 222);
    const static glm::vec4 COLOR_LIGHT_YELLOW = rgb255(255, 255, 224);
    const static glm::vec4 COLOR_LIME = rgb255(0, 255, 0);
    const static glm::vec4 COLOR_LIME_GREEN = rgb255(50, 205, 50);
    const static glm::vec4 COLOR_LINEN = rgb255(250, 240, 230);
    const static glm::vec4 COLOR_MAGENTA = rgb255(255, 0, 255);
    const static glm::vec4 COLOR_MAROON = rgb255(128, 0, 0);
    const static glm::vec4 COLOR_MEDIUM_AQUAMARINE = rgb255(102, 205, 170);
    const static glm::vec4 COLOR_MEDIUM_BLUE = rgb255(0, 0, 205);
    const static glm::vec4 COLOR_MEDIUM_ORCHID = rgb255(186, 85, 211);
    const static glm::vec4 COLOR_MEDIUM_PURPLE = rgb255(147, 112, 219);
    const static glm::vec4 COLOR_MEDIUM_SEA_GREEN = rgb255(60, 179, 113);
    const static glm::vec4 COLOR_MEDIUM_SLATE_BLUE = rgb255(123, 104, 238);
    const static glm::vec4 COLOR_MEDIUM_SPRING_GREEN = rgb255(0, 250, 154);
    const static glm::vec4 COLOR_MEDIUM_TURQUOISE = rgb255(72, 209, 204);
    const static glm::vec4 COLOR_MEDIUM_VIOLET_RED = rgb255(199, 21, 133);
    const static glm::vec4 COLOR_MIDNIGHT_BLUE = rgb255(25, 25, 112);
    const static glm::vec4 COLOR_MINT_CREAM = rgb255(245, 255, 250);
    const static glm::vec4 COLOR_MISTY_ROSE = rgb255(255, 228, 225);
    const static glm::vec4 COLOR_MOCCASIN = rgb255(255, 228, 181);
    const static glm::vec4 COLOR_NAVAJO_WHITE = rgb255(255, 222, 173);
    const static glm::vec4 COLOR_NAVY = rgb255(0, 0, 128);
    const static glm::vec4 COLOR_OLD_LACE = rgb255(253, 245, 230);
    const static glm::vec4 COLOR_OLIVE = rgb255(128, 128, 0);
    const static glm::vec4 COLOR_OLIVE_DRAB = rgb255(107, 142, 35);
    const static glm::vec4 COLOR_ORANGE = rgb255(255, 165, 0);
    const static glm::vec4 COLOR_ORANGE_RED = rgb255(255, 69, 0);
    const static glm::vec4 COLOR_ORCHID = rgb255(218, 112, 214);
    const static glm::vec4 COLOR_PALE_GOLDENROD = rgb255(238, 232, 170);
    const static glm::vec4 COLOR_PALE_GREEN = rgb255(152, 251, 152);
    const static glm::vec4 COLOR_PALE_TURQUOISE = rgb255(175, 238, 238);
    const static glm::vec4 COLOR_PALE_VIOLET_RED = rgb255(219, 112, 147);
    const static glm::vec4 COLOR_PAPAYA_WHIP = rgb255(255, 239, 213);
    const static glm::vec4 COLOR_PEACH_PUFF = rgb255(255, 218, 185);
    const static glm::vec4 COLOR_PERU = rgb255(205, 133, 63);
    const static glm::vec4 COLOR_PINK = rgb255(255, 192, 203);
    const static glm::vec4 COLOR_PLUM = rgb255(221, 160, 221);
    const static glm::vec4 COLOR_POWDER_BLUE = rgb255(176, 224, 230);
    const static glm::vec4 COLOR_PURPLE = rgb255(128, 0, 128);
    const static glm::vec4 COLOR_RED = rgb255(255, 0, 0);
    const static glm::vec4 COLOR_ROSY_BROWN = rgb255(188, 143, 143);
    const static glm::vec4 COLOR_ROYAL_BLUE = rgb255(65, 105, 225);
    const static glm::vec4 COLOR_SADDLE_BROWN = rgb255(139, 69, 19);
    const static glm::vec4 COLOR_SALMON = rgb255(250, 128, 114);
    const static glm::vec4 COLOR_SANDY_BROWN = rgb255(244, 164, 96);
    const static glm::vec4 COLOR_SEA_GREEN = rgb255(46, 139, 87);
    const static glm::vec4 COLOR_SEASHELL = rgb255(255, 245, 238);
    const static glm::vec4 COLOR_SIENNA = rgb255(160, 82, 45);
    const static glm::vec4 COLOR_SILVER = rgb255(192, 192, 192);
    const static glm::vec4 COLOR_SKY_BLUE = rgb255(135, 206, 235);
    const static glm::vec4 COLOR_SLATE_BLUE = rgb255(106, 90, 205);
    const static glm::vec4 COLOR_SLATE_GRAY = rgb255(112, 128, 144);
    const static glm::vec4 COLOR_SLATE_GREY = rgb255(112, 128, 144);
    const static glm::vec4 COLOR_SNOW = rgb255(255, 250, 250);
    const static glm::vec4 COLOR_SPRING_GREEN = rgb255(0, 255, 127);
    const static glm::vec4 COLOR_STEEL_BLUE = rgb255(70, 130, 180);
    const static glm::vec4 COLOR_TAN = rgb255(210, 180, 140);
    const static glm::vec4 COLOR_TEAL = rgb255(0, 128, 128);
    const static glm::vec4 COLOR_THISTLE = rgb255(216, 191, 216);
    const static glm::vec4 COLOR_TOMATO = rgb255(255, 99, 71);
    const static glm::vec4 COLOR_TURQUOISE = rgb255(64, 224, 208);
    const static glm::vec4 COLOR_VIOLET = rgb255(238, 130, 238);
    const static glm::vec4 COLOR_WHEAT = rgb255(245, 222, 179);
    const static glm::vec4 COLOR_WHITE = rgb255(255, 255, 255);
    const static glm::vec4 COLOR_WHITE_SMOKE = rgb255(245, 245, 245);
    const static glm::vec4 COLOR_YELLOW = rgb255(255, 255, 0);
    const static glm::vec4 COLOR_YELLOW_GREEN = rgb255(154, 205, 50);
}