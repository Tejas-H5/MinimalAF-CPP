#include"Window.h"
#define _USE_MATH_DEFINES
#include<math.h>

using namespace af;

int af::init() {
    if (!glfwInit()) {
        print("couldn't init glfw");
        return -1;
    }


    return 0;
}

void af::uninit() {
    print("glfwTerminate();");
    glfwTerminate();
}


void af::Window::onResize(GLFWwindow* window, int width, int height) {
    windowSize = SizeI(width, height);
    glViewport(0, 0, width, height);

    // TODO: other stuff

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window::s_instance->onResize(window, width, height);
}

Window* Window::s_instance = NULL;

Window::Window(int w, int h, const std::string& title) : meshOutput(NULL) {
    window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

    if (!window) {
        print("window wasn't created");
        return;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Window::s_instance = this;

    // TODO: fine-tune this size
    meshOutput = new BufferedMeshOutput(8 * 4096, 8 * 4096);
    internalShader = new InternalShader();

    shaderState.currentShader = internalShader;
}


Window::~Window() {
    glfwDestroyWindow(window);
    window = nullptr;

    Window::s_instance = NULL;

    delete meshOutput;
    delete internalShader;
}

void af::Window::run() {
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        print("glewInit() != GLEW_OK");
        return;
    }

    initialize();

    // this is an update/render loop.
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: make render and update happen at different intervals
        // TODO: also give them the deltatime somehow. static class or pass it in as an arg here
        update();
        render();

        glfwSwapBuffers(window);
    }
}

void af::Window::setState(WindowMode mode) {
    if (mode == WindowMode::MAXIMIZED) {
        glfwMaximizeWindow(window);
    }
    else if (mode == WindowMode::FULLSCREEN) {
        print("TODO: implement window fullscreening");
    }
    else if (mode == WindowMode::MINIMIZED) {
        glfwIconifyWindow(window);
    }
    else if (mode == WindowMode::WINDOWED) {
        glfwRestoreWindow(window);
    }
}

void af::Window::setSize(int w, int h) {
    glfwSetWindowSize(window, w, h);
}

void af::Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(window, title.c_str());
}

SizeI af::Window::getSize() {
    SizeI windowSize;
    glfwGetWindowSize(window, &windowSize.width, &windowSize.height);

    return windowSize;
}

// ---- Rendering functions
static Vertex vertex2D(float x, float y, float u, float v) {
    return Vertex(vec3(x, y, 0), vec2(0, v));
}

void af::Window::drawTriangle(Vertex v1, Vertex v2, Vertex v3) {
    meshOutput->flushIfRequired(3, 3);

    uint i1 = meshOutput->addVertex(v1);
    uint i2 = meshOutput->addVertex(v2);
    uint i3 = meshOutput->addVertex(v3);

    meshOutput->makeTriangle(i1, i2, i3);
}

void af::Window::drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, float u0, float v0, float u1, float v1, float u2, float v2) {
    Vertex vert1 = vertex2D(x0, y0, u0, v0);
    Vertex vert2 = vertex2D(x1, y1, u1, v1);
    Vertex vert3 = vertex2D(x2, y2, u2, v2);

    drawTriangle(vert1, vert2, vert3);
}

void af::Window::drawTriangleOutline(float thickness, float x0, float y0, float x1, float y1, float x2, float y2) {
    beginNLine(x0, y0, thickness, CapType::None);
    continueNLine(x1, y1);
    continueNLine(x2, y2);
    endNLine(x0, y0);
}

void af::Window::drawQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4) {
    meshOutput->flushIfRequired(4, 6);

    uint i1 = meshOutput->addVertex(v1);
    uint i2 = meshOutput->addVertex(v2);
    uint i3 = meshOutput->addVertex(v3);
    uint i4 = meshOutput->addVertex(v4);

    meshOutput->makeTriangle(i1, i2, i3);
    meshOutput->makeTriangle(i3, i4, i1);
}

void af::Window::drawQuad2D(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3) {
    drawQuad(
        vertex2D(x0, y0, u0, v0),
        vertex2D(x1, y1, u1, v1),
        vertex2D(x2, y2, u2, v2),
        vertex2D(x3, y3, u3, v3)
    );
}

void af::Window::drawRect(float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1) {
    drawQuad2D(
        x0, y0,
        x1, y0,
        x1, y1,
        x0, y1,
        u0, v1,
        u1, v1,
        u1, v0,
        u0, v0
    );
}

void af::Window::drawRect(Rect rect, Rect uvs) {
    drawRect(rect.X0, rect.Y0, rect.X1, rect.Y1, uvs.X0, uvs.Y0, uvs.X1, uvs.Y1);
}

void af::Window::drawRect(Rect rect) {
    drawRect(rect.X0, rect.Y0, rect.X1, rect.Y1);
}

void af::Window::drawRectOutline(float thickness, Rect rect) {
    drawRectOutline(thickness, rect.X0, rect.Y0, rect.X1, rect.Y1);
}

void af::Window::drawRectOutline(float thickness, float x0, float y0, float x1, float y1) {
    drawRect(x0 - thickness, y0 - thickness, x1, y0);
    drawRect(x0, y1, x1 + thickness, y1 + thickness);

    drawRect(x0 - thickness, y0, x0, y1 + thickness);
    drawRect(x1, y0 - thickness, x1 + thickness, y1);
}

void af::Window::beginNGon2D(float centerX, float centerY, int n) {
    beginNGon(vertex2D(centerX, centerY, 0, 0), n);
}

void af::Window::beginNGon(Vertex v1, int n) {
    if (n < 3)
        n = 3;

    ngonState.polygonBegun = true;

    meshOutput->flushIfRequired(n, 3 * (n - 2));

    ngonState.polygonFirst = meshOutput->addVertex(v1);
    ngonState.firstVertex = v1;
    ngonState.polygonCount = 1;
}

void af::Window::continueNGon2D(float centerX, float centerY) {
    continueNGon(vertex2D(centerX, centerY, 0, 0));
}

void af::Window::continueNGon(Vertex v) {
    if (ngonState.polygonBegun == false) {
        return;
    }

    if (ngonState.polygonCount == 1) {
        // just append the second vertex, we can't make a triangle yet
        ngonState.polygonSecond = meshOutput->addVertex(v);
        ngonState.secondVertex = v;
        ngonState.polygonCount = 2;
        return;
    }


    if (meshOutput->flushIfRequired(1, 3)) {
        ngonState.polygonFirst = meshOutput->addVertex(ngonState.firstVertex);
        ngonState.polygonSecond = meshOutput->addVertex(ngonState.secondVertex);
    }


    ngonState.polygonCount++;

    uint polygonThird = meshOutput->addVertex(v);
    meshOutput->makeTriangle(ngonState.polygonFirst, ngonState.polygonSecond, polygonThird);

    ngonState.polygonSecond = polygonThird;
    ngonState.secondVertex = v;
}

void af::Window::endNGon() {
    ngonState.polygonBegun = false;
}

void af::Window::beginNLine(float x, float y, float thickness, CapType cap) {
    if (!nlineState.canStart) {
        continueNLine(x, y);
        return;
    }

    nlineState.thickness = thickness;
    nlineState.lastLast = vec2(x, y);
    nlineState.last = nlineState.lastLast;

    nlineState.capType = cap;
    nlineState.count = 1;
}


static void nlineCalculateLineParameters(const NLineDrawState& nlineState, float x, float y, vec2* dir, vec2* perp) {
    dir[0] = vec2(x, y) - nlineState.last;

    float mag = glm::length(dir[0]);

    perp[0] = vec2(-nlineState.thickness * dir[0].y / mag, nlineState.thickness * dir[0].x / mag);
}


void af::Window::nlineStartLineSegment(float x, float y, vec2 dir, vec2 perp) {
    Vertex v1 = vertex2D(nlineState.last.x + perp.x, nlineState.last.y + perp.y, 0, 0);
    Vertex v2 = vertex2D(nlineState.last.x - perp.x, nlineState.last.y - perp.y, 0, 0);

    meshOutput->flushIfRequired(2, 0);

    nlineState.lastV1 = meshOutput->addVertex(v1);
    nlineState.lastV2 = meshOutput->addVertex(v2);

    nlineState.lastV1Vert = v1;
    nlineState.lastV2Vert = v2;
    nlineState.lastPerp = perp;


    float startAngle = atan2(dir.x, dir.y) + M_PI / 2.0f;
    drawLineCap(nlineState.last.x, nlineState.last.y, nlineState.thickness, nlineState.capType, startAngle);
}

// TODO: clean this up, or use a better algorithm.
void af::Window::nlineMoveLineSegmentInDirectionOf(float x, float y, bool useAverage, vec2 dir, vec2 perp) {
    vec2 perpUsed;

    if (useAverage) {
        perpUsed = (perp + nlineState.lastPerp) / 2.0f;

        perpUsed = nlineState.thickness * perpUsed / glm::length(perpUsed);
    }
    else {
        perpUsed = perp;
    }


    Vertex v3 = vertex2D(nlineState.last.x + perpUsed.x, nlineState.last.y + perpUsed.y, 0, 0);
    Vertex v4 = vertex2D(nlineState.last.x - perpUsed.x, nlineState.last.y - perpUsed.y, 0, 0);

    if (meshOutput->flushIfRequired(4, 6)) {
        nlineState.lastV1 = meshOutput->addVertex(nlineState.lastV1Vert);
        nlineState.lastV2 = meshOutput->addVertex(nlineState.lastV2Vert);
    }


    //check if v3 and v4 intersect with v1 and v2
    vec2 lastDir = vec2(-nlineState.lastPerp.y, nlineState.lastPerp.x);
    vec2 vec1 = nlineState.last + perpUsed - nlineState.lastLast;
    vec2 vec2 = nlineState.last - perpUsed - nlineState.lastLast;

    bool v3IsArtifacting = glm::dot(vec1, lastDir) > 0;
    bool v4IsArtifacting = glm::dot(vec2, lastDir) > 0;

    if (v3IsArtifacting || v4IsArtifacting) {
        if (v3IsArtifacting) {
            nlineState.lastV4 = meshOutput->addVertex(v4);
            meshOutput->makeTriangle(nlineState.lastV1, nlineState.lastV2, nlineState.lastV4);
            nlineState.lastV2 = nlineState.lastV4;
            nlineState.lastV2Vert = v4;
        }
        else if (v4IsArtifacting) {
            nlineState.lastV3 = meshOutput->addVertex(v3);
            meshOutput->makeTriangle(nlineState.lastV1, nlineState.lastV2, nlineState.lastV3);
            nlineState.lastV1 = nlineState.lastV3;
            nlineState.lastV1Vert = v3;
        }
    }
    else {
        nlineState.lastV3 = meshOutput->addVertex(v3);
        nlineState.lastV4 = meshOutput->addVertex(v4);

        meshOutput->makeTriangle(nlineState.lastV1, nlineState.lastV2, nlineState.lastV3);
        meshOutput->makeTriangle(nlineState.lastV3, nlineState.lastV2, nlineState.lastV4);

        nlineState.lastV1 = nlineState.lastV3;
        nlineState.lastV2 = nlineState.lastV4;
        nlineState.lastV1Vert = v3;
        nlineState.lastV2Vert = v4;
    }

    nlineState.lastPerp = perp;
}

void af::Window::continueNLine(float x, float y, bool useAverage) {
    vec2 dir, perp;
    nlineCalculateLineParameters(nlineState, x, y, &dir, &perp);

    float mag = glm::length(dir);

    if (mag < 0.00001f)
        return;


    if (nlineState.count == 1) {
        nlineStartLineSegment(x, y, dir, perp);
    }
    else {
        nlineMoveLineSegmentInDirectionOf(x, y, useAverage, dir, perp);
    }

    nlineState.lastLast = nlineState.last;

    nlineState.last = vec2(x, y);
    nlineState.count++;
}

void af::Window::endNLine(float x, float y) {
    if (!nlineState.canEnd) {
        continueNLine(x, y);
        return;
    }

    vec2 dir = vec2(x, y) - nlineState.last;

    float mag = glm::length(dir);
    if (mag < 0.001f) {
        dir = vec2(x, y) - nlineState.lastLast;
    }

    continueNLine(x, y);
    continueNLine(x + dir.x, y + dir.y, false);

    nlineState.last = vec2(x, y);

    float startAngle = atan2(dir.x, dir.y) + M_PI / 2;

    if (nlineState.count == 1) {
        drawLineCap(nlineState.last.x, nlineState.last.y, nlineState.thickness, nlineState.capType, startAngle);
    }

    drawLineCap(nlineState.last.x, nlineState.last.y, nlineState.thickness, nlineState.capType, startAngle + M_PI);

    nlineState.canStart = true;
}

void af::Window::disableEndingNLine() {
    nlineState.canEnd = false;
    nlineState.canStart = false;
}

void af::Window::enableEndingNLine() {
    nlineState.canEnd = true;
}

static int arcGetEdgeCount(const ArcDrawState& arcState, float radius, float startAngle, float endAngle) {
    float deltaAngle = arcState.circleEdgeLength / radius;
    int edgeCount = (int)((endAngle - startAngle) / deltaAngle) + 1;

    if (edgeCount > arcState.maxCircleEdgeCount) {
        edgeCount = arcState.maxCircleEdgeCount;
    }

    return edgeCount;
}

void af::Window::drawArc(float xCenter, float yCenter, float radius, float startAngle, float endAngle) {
    int edgeCount = arcGetEdgeCount(arcState, radius, startAngle, endAngle);

    drawArc(xCenter, yCenter, radius, startAngle, endAngle, edgeCount);
}

void af::Window::drawArc(float xCenter, float yCenter, float radius, float startAngle, float endAngle, int edgeCount) {
    if (edgeCount < 0)
        return;

    float deltaAngle = (endAngle - startAngle) / edgeCount;

    beginNGon2D(xCenter, yCenter, edgeCount + 2);

    for (float angle = endAngle; angle > startAngle - deltaAngle + 0.001f; angle -= deltaAngle) {
        float X = xCenter + radius * sin(angle);
        float Y = yCenter + radius * cos(angle);

        continueNGon2D(X, Y);
    }

    endNGon();
}

void af::Window::drawArcOutline(float thickness, float x0, float y0, float r, float startAngle, float endAngle) {
    int edgeCount = arcGetEdgeCount(arcState, r, startAngle, endAngle);

    drawArcOutline(thickness, x0, y0, r, startAngle, endAngle, edgeCount);
}

void af::Window::drawArcOutline(float thickness, float xCenter, float yCenter, float radius, float startAngle, float endAngle, int edgeCount) {
    if (edgeCount < 0)
        return;

    thickness /= 2.0f;
    radius += thickness / 2.0f;

    float deltaAngle = (endAngle - startAngle) / edgeCount;

    bool first = true;
    for (float angle = startAngle; angle < endAngle + deltaAngle - 0.001f; angle += deltaAngle) {
        float X = xCenter + radius * sin(angle);
        float Y = yCenter + radius * cos(angle);

        if (first) {
            beginNLine(X, Y, thickness, CapType::None);
            first = false;
        }
        else if (angle + deltaAngle < endAngle + 0.00001f) {
            continueNLine(X, Y);
        }
        else {
            endNLine(X, Y);
        }
    }
}

void af::Window::drawCircle(float x0, float y0, float r, int edges) {
    drawArc(x0, y0, r, 0, M_PI * 2.0f, edges);
}

void af::Window::drawCircle(float x0, float y0, float r) {
    drawArc(x0, y0, r, 0, M_PI * 2.0f);
}

void af::Window::drawCircleOutline(float thickness, float x0, float y0, float r, int edges) {
    drawArcOutline(thickness, x0, y0, r, 0, M_PI * 2.0f, edges);
}

void af::Window::drawCircleOutline(float thickness, float x0, float y0, float r) {
    drawArcOutline(thickness, x0, y0, r, 0, M_PI * 2.0f);
}

void af::Window::drawLine(float x0, float y0, float x1, float y1, float thickness, CapType cap) {
    thickness /= 2.0f;

    float dirX = x1 - x0;
    float dirY = y1 - y0;
    float mag = sqrt(dirX * dirX + dirY * dirY);

    float perpX = -thickness * dirY / mag;
    float perpY = thickness * dirX / mag;


    drawQuad2D(
        x0 + perpX, y0 + perpY,
        x0 - perpX, y0 - perpY,
        x1 - perpX, y1 - perpY,
        x1 + perpX, y1 + perpY
    );


    float startAngle = atan2f(dirX, dirY) + M_PI / 2;
    drawLineCap(x0, y0, thickness, cap, startAngle);
    drawLineCap(x1, y1, thickness, cap, startAngle + M_PI);
}

void af::Window::drawLineOutline(float outlineThickness, float x0, float y0, float x1, float y1, float thickness, CapType cap) {
    thickness /= 2;

    float dirX = x1 - x0;
    float dirY = y1 - y0;
    float mag = sqrt(dirX * dirX + dirY * dirY);

    float perpX = -(thickness + outlineThickness / 2.0f) * dirY / mag;
    float perpY = (thickness + outlineThickness / 2.0f) * dirX / mag;


    //Draw the outline using a NASCAR path
    beginNLine(x1 - perpX, y1 - perpY, outlineThickness, CapType::None);

    disableEndingNLine();

    //immediateModeDrawer.NLine.AppendToPolyLine(x0 - perpX, y0 - perpY);

    float startAngle = atan2(dirX, dirY) + M_PI / 2.0f;
    drawLineCapOutline(outlineThickness, x0, y0, thickness, cap, startAngle);

    //immediateModeDrawer.NLine.AppendToPolyLine(0, 0);

    //immediateModeDrawer.NLine.AppendToPolyLine(x1 + perpX, y1 + perpY);

    enableEndingNLine();
    drawLineCapOutline(outlineThickness, x1, y1, thickness, cap, startAngle + M_PI);
}

void af::Window::drawLineCap(float x0, float y0, float radius, CapType cap, float startAngle) {
    switch (cap) {
    case CapType::Circle: {
        drawLineCircleCap(x0, y0, radius, startAngle);
        break;
    }
    default:
        break;
    }
}

void af::Window::drawLineCircleCap(float x0, float y0, float thickness, float angle) {
    drawArc(x0, y0, thickness, angle, angle + M_PI);
}

void af::Window::drawLineCapOutline(float outlineThickness, float x0, float y0, float radius, CapType cap, float startAngle) {
    switch (cap) {
    case CapType::Circle:
        drawLineCircleCapOutline(outlineThickness, x0, y0, radius, startAngle);
        break;
    default:
        drawLineDefaultCapOutline(outlineThickness, x0, y0, radius, startAngle);
        break;
    }
}

void af::Window::drawLineDefaultCapOutline(float thickness, float x0, float y0, float radius, float angle) {
    // draws an outline for a 1 edge arc. lmao
    drawArcOutline(thickness, x0, y0, radius, angle, angle + M_PI, 1);
}

void af::Window::drawLineCircleCapOutline(float thickness, float x0, float y0, float radius, float angle) {
    drawArcOutline(thickness, x0, y0, radius, angle, angle + M_PI);
}

void af::Window::setTextFont(std::string name, int size) {

}

float af::Window::getTextWidth() {
    return 0.0f;
}

float af::Window::getTextHeight() {
    return 0.0f;
}

float af::Window::getTextWidth(char c) {
    return 0.0f;
}

float af::Window::getTextHeight(char c) {
    return 0.0f;
}

vec2 af::Window::getTextSize(char c) {
    return vec2();
}

Texture af::Window::getTextTexture() {
    return Texture();
}

vec2 af::Window::drawText(std::string text, float startX, float startY, HAlign hAlign, VAlign vAlign, float scale) {
    return vec2();
}

vec2 af::Window::drawText(std::string text, float startX, float startY, float scale) {
    return vec2();
}

vec2 af::Window::drawText(std::string text, int start, int end, float startX, float startY, float scale) {
    return vec2();
}

float af::Window::getTextStringHeight(std::string s) {
    return 0.0f;
}

float af::Window::getTextStringHeight(std::string s, int start, int end) {
    return 0.0f;
}

float af::Window::getTextStringWidth(std::string s) {
    return 0.0f;
}

vec4 af::Window::getClearColor() {
    return clearColor;
}

void af::Window::setClearColor(vec4 color) {
    clearColor = color;
    glClearColor(color.x, color.y, color.z, color.w);
}

void af::Window::clear() {
    glStencilMask(1);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void af::Window::flush() {
    meshOutput->flush();
}

void af::Window::swapBuffers() {
    flush();
    // s_framebufferManager.Use(null);
    // s_shaderManager.SetModelMatrix(Matrix4.Identity);

    glfwSwapBuffers(window);

    meshOutput->timesIndexThresholdReached = 0;
    meshOutput->timesVertexThresholdReached = 0;
}

void af::Window::setViewport(Rect screenRect) {
    screenRect = screenRect.rectified();

    glViewport((int)screenRect.X0, (int)screenRect.Y0, (int)screenRect.getWidth(), (int)screenRect.getHeight());
}

void af::Window::cartesian2D(float scaleX, float scaleY, float offsetX, float offsetY) {
    flush();

    float width = scaleX * windowSize.width;
    float height = scaleY * windowSize.height;

    mat4 viewMatrix = translation(vec3(offsetX - width / 2, offsetY - height / 2, 0));

    mat4 projectionMatrix = scale(vec3(2.0f / width, 2.0f / height, 1));

    // s_shaderManager.SetViewMatrix(viewMatrix);
    setProjection(projectionMatrix);
    glDepthFunc(GL_LEQUAL);
}

void af::Window::viewLookAt(vec3 position, vec3 target, vec3 up) {
    mat4 lookAt = glm::lookAt(position, target, up);

    // s_shaderManager.SetViewMatrix(lookAt);
}

void af::Window::viewOrientation(vec3 position, quat rot) {
    mat4 orienation = translation(-position);
    orienation *= rotation(rot);

    // s_shaderManager.SetViewMatrix(orienation);
}

void af::Window::perspective(float fovy, float aspect, float depthNear, float depthFar, float centerX, float centerY) {
    mat4 perspective = glm::perspective(fovy, aspect, depthNear, depthFar) *
        translation(vec3(centerX / (float)windowSize.width, centerY / (float)windowSize.height, 0));

    setProjection(perspective);
}

void af::Window::orthographic(float width, float height, float depthNear, float depthFar, float centerX, float centerY) {
    // TODO: test this function, it is different from what we had before in the C# implementation
    mat4 ortho = glm::ortho(0.0f, width, 0.0f, height, depthNear, depthFar);
    ortho = ortho * translation(vec3(centerX / (float)windowSize.width, centerY / (float)windowSize.height, 0));

    setProjection(ortho);
}

void af::Window::setProjection(mat4 matrix) {
    // s_shaderManager.SetProjectionMatrix(matrix);
}

void af::Window::setBackfaceCulling(bool onOrOff) {
    if (onOrOff) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

void af::Window::setTransform(mat4 matrix) {
    // s_shaderManager.SetModelMatrix(matrix);
}

void af::Window::setDrawColor(vec4 col) {
    //if (s_internalShader.Color == col)
//        return;

    flush();

    // s_internalShader.Color = col;
}


/// <summary>
/// Clears and enables the stencil buffer, doesn't disable colour writes.
/// </summary>
/// <param name="inverseStencil">If this parameter is set to true, the buffer will be cleared to 1s and
/// we will be drawing 0s to the stencil buffer, and vice versa.</param>
void af::Window::startStencillingWhileDrawing(bool inverseStencil) {
    startStencilling(true, inverseStencil);
}


/// <summary>
/// Clears and enables the stencil buffer, and then disables colour writes
/// </summary>
/// <param name="inverseStencil">If this parameter is set to true, the buffer will be cleared to 1s and
/// we will be drawing 0s to the stencil buffer, and vice versa.</param>
void af::Window::startStencillingWithoutDrawing(bool inverseStencil) {
    startStencilling(false, inverseStencil);
}


void af::Window::startStencilling(bool canDraw, bool inverseStencil) {
    flush();

    if (!canDraw) {
        glColorMask(false, false, false, false);
    }

    if (inverseStencil) {
        glClearStencil(1);
    }
    else {
        glClearStencil(0);
    }

    glStencilMask(1);
    glClear(GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    if (inverseStencil) {
        glStencilFunc(GL_ALWAYS, 0, 0);
    }
    else {
        glStencilFunc(GL_ALWAYS, 1, 1);
    }
}

void af::Window::startUsingStencil() {
    flush();

    glColorMask(true, true, true, true);
    glStencilFunc(GL_NOTEQUAL, 1, 1);
    glStencilMask(0);
}

void af::Window::liftStencil() {
    flush();

    glDisable(GL_STENCIL_TEST);
}

void af::Window::setModelMatrix(mat4 matrix) {
    flush();

    shaderState.model = matrix;
    shaderState.currentShader->setModel(matrix);
}

void af::Window::setProjectionMatrix(mat4 matrix) {
    flush();

    shaderState.projection = matrix;
    shaderState.currentShader->setProjection(matrix);
}

void af::Window::setViewMatrix(mat4 matrix) {
    flush();

    shaderState.view = matrix;
    shaderState.currentShader->setView(matrix);
}

void af::Window::useShader(Shader* s, bool updateUniforms) {
    if (s == shaderState.currentShader)
        return;

    shaderState.currentShader = s;
    
    if (updateUniforms) {
        shaderState.currentShader->setModel(shaderState.model);
        shaderState.currentShader->setView(shaderState.view);
        shaderState.currentShader->setModel(shaderState.projection);
    }
}
