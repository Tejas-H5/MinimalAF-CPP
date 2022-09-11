#pragma once
#include "../helpers.h"

namespace af {

    struct Rect {
    public:
        float x0;
        float y0;
        float x1;
        float y1;

        Rect() : x0(0), y0(0), x1(0), y1(0) {}

        Rect(float x0, float y0, float x1, float y1)
            : x0(x0), y0(y0), x1(x1), y1(y1) {}

        inline Rect resizedWidth(float newWidth, float center = 0) {
            Rect newRect = Rect(*this);

            float delta = getWidth() - newWidth;
            newRect.x0 += delta * center;
            newRect.x1 -= delta * (1.0f - center);

            return newRect;
        }

        inline Rect ResizedHeight(float newHeight, float center = 0) {
            Rect newRect = Rect(x0, y0, x1, y1);

            float delta = getHeight() - newHeight;
            newRect.y0 += delta * center;
            newRect.y1 -= delta * (1.0f - center);

            return newRect;
        }

        inline float getLeft() { return x0 < x1 ? x0 : x1; }
        inline float getRight() { return x0 < x1 ? x1 : x0; }
        inline float getBottom() { return y0 < y1 ? y0 : y1; }
        inline float getTop() { return y0 < y1 ? y1 : y0; }
        inline float getCenterX() { return x0 + (x1 - x0) * 0.5f; }
        inline float getCenterY() { return y0 + (y1 - y0) * 0.5f; }
        inline float getSmallerDimension() {
            float w = getWidth();
            float h = getHeight();
            return w < h ? w : h;
        }

        inline float getLargerDimension() {
            float w = getWidth();
            float h = getHeight();
            return w > h ? w : h;
        }

        inline Rect scaled(float sx, float sy) {
            return Rect(x0 * sx, y0 * sy, x1 * sx, y1 * sy);
        }

        inline Rect scaled(float s) {
            return scaled(s, s);
        }

        inline Rect rectified() {
            return Rect(getLeft(), getBottom(), getRight(), getTop());
        }

        inline float getWidth() { return getRight() - getLeft(); }
        inline float getHeight() { return getTop() - getBottom(); }

        inline bool isInvalid() { return x0 > x1 || y0 > y1; }

        inline Rect moved(float x, float y) {
            return Rect(x0 + x, y0 + y, x1 + x, y1 + y);
        }

        inline Rect intersect(Rect other) {
            return Rect(
                max(getLeft(), other.getLeft()),
                max(getBottom(), other.getBottom()),
                min(getRight(), other.getRight()),
                min(getTop(), other.getTop())
            );
        }

        inline Rect inset(float margin) {
            return inset(margin, margin, margin, margin);
        }

        inline Rect inset(float left, float bottom, float right, float top) {
            return Rect(x0 + left, y0 + bottom, x1 - right, y1 - top);
        }

        inline glm::vec2 constrain(glm::vec2 point) {
            point.x = clamp(point.x, x0, x1);
            point.y = clamp(point.y, y0, y1);

            return point;
        }

        inline Rect rectTwoPoint(float x0, float y0, float x1, float y1) {
            return Rect(x0, y0, x1, y1);
        }

        inline Rect rectPivotSize(float width, float height, float xPivot, float yPivot) {
            return Rect(
                -xPivot * width,
                -yPivot * height,
                (1.0f - xPivot) * width,
                (1.0f - yPivot) * height
            );
        }
    };
}
