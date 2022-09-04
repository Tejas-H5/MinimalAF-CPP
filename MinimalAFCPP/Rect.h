#pragma once
#include<string>
#include "helpers.h"

namespace af {
    struct Rect {
    public:
        float X0;
        float Y0;
        float X1;
        float Y1;

        Rect(float x0, float y0, float x1, float y1)
            : X0(x0), Y0(y0), X1(x1), Y1(y1) {}

        inline Rect resizedWidth(float newWidth, float center = 0) {
            Rect newRect = Rect(*this);

            float delta = getWidth() - newWidth;
            newRect.X0 += delta * center;
            newRect.X1 -= delta * (1.0f - center);

            return newRect;
        }

        inline Rect ResizedHeight(float newHeight, float center = 0) {
            Rect newRect = Rect(X0, Y0, X1, Y1);

            float delta = getHeight() - newHeight;
            newRect.Y0 += delta * center;
            newRect.Y1 -= delta * (1.0f - center);

            return newRect;
        }

        inline float getLeft() { return X0 < X1 ? X0 : X1; }
        inline float getRight() { return X0 < X1 ? X1 : X0; }
        inline float getBottom() { return Y0 < Y1 ? Y0 : Y1; }
        inline float getTop() { return Y0 < Y1 ? Y1 : Y0; }
        inline float getCenterX() { return X0 + (X1 - X0) * 0.5f; }
        inline float getCenterY() { return Y0 + (Y1 - Y0) * 0.5f; }
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
            return Rect(X0 * sx, Y0 * sy, X1 * sx, Y1 * sy);
        }

        inline Rect scaled(float s) {
            return scaled(s, s);
        }

        inline Rect rectified() {
            return Rect(getLeft(), getBottom(), getRight(), getTop());
        }

        inline float getWidth() { return getRight() - getLeft(); }
        inline float getHeight() { return getTop() - getBottom(); }

        inline bool isInvalid() { return X0 > X1 || Y0 > Y1; }

        inline Rect moved(float x, float y) {
            return Rect(X0 + x, Y0 + y, X1 + x, Y1 + y);
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
            return Rect(X0 + left, Y0 + bottom, X1 - right, Y1 - top);
        }

        inline glm::vec2 constrain(glm::vec2 point) {
            point.x = clamp(point.x, X0, X1);
            point.y = clamp(point.y, Y0, Y1);

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