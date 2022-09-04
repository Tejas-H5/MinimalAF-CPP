#pragma once

#include "Datatypes.h"
#include "Texture.h"
#include "helpers.h"
#include "BufferedMeshOutput.h"

#include"GL/glew.h"
#include "GLFW/glfw3.h"

#include<string>

namespace af {
	int init();
	void uninit();

	enum class WindowMode {
		WINDOWED,
		FULLSCREEN,
		MAXIMIZED,
		MINIMIZED
	};

	struct NGonDrawState {
		bool polygonBegun = false;
		uint polygonFirst;
		uint polygonSecond = 0;
		uint polygonCount = 0;
		Vertex firstVertex;
		Vertex secondVertex;
	};

	struct ArcDrawState {
		int circleEdgeLength, maxCircleEdgeCount;
		ArcDrawState() : circleEdgeLength(5), maxCircleEdgeCount(32) {}
	};

	// used for nLine. or rather, it will be when I implement it correctly
	struct NLineDrawState {
		bool canStart = true;
		bool canEnd = true;

		float thickness = 0;

		vec2 lastLast;
		vec2 last;
		vec2 lastPerp;

		uint lastV1;
		uint lastV2;
		Vertex lastV1Vert;
		Vertex lastV2Vert;
		uint lastV3;
		uint lastV4;
		uint count = 0;

		CapType capType;
	};

	class Window {
	private:
		GLFWwindow* window;
		
		SizeI windowSize;

		float current2DDepth = 0;
		Rect currentClippingRect;
		vec4 clearColor;

		NGonDrawState ngonState;
		NLineDrawState nlineState;
		ArcDrawState arcState;

		BufferedMeshOutput* meshOutput;
																																										
/*
			internal static TextureManager Texture = > s_textureManager;
			internal static FramebufferManager Framebuffer = > s_framebufferManager;
			public static ShaderManager Shader = > s_shaderManager;
			private static TextDrawer<Vertex> s_textDrawer;
			private static MeshOutputStream<Vertex> s_meshOutputStream;
			private static ImmediateMode2DDrawer<Vertex> s_imDrawer;
			private static readonly VertexCreator s_vertexCreator = new VertexCreator();
			private static TextureManager s_textureManager;
			private static FramebufferManager s_framebufferManager;
			private static ShaderManager s_shaderManager;
			private static InternalShader s_internalShader;
			public static Texture InternalFontTexture = > s_textDrawer.ActiveFont.FontTexture;
			public static int TimesVertexThresholdReached {
			public static int TimesIndexThresholdReached {
			public static float VertexToIndexRatio = > (float)TimesVertexThresholdReached / (float)TimesIndexThresholdReached;
			internal static void SetScreenWidth(int width, int height) {
			internal static void Init(IGLFWGraphicsContext context) {
			internal static Color GetClearColor() {
			internal static void SetClearColor(Color color) {
			internal static void Clear() {
			internal static void Clear(Color col) {
			internal static void Flush() {
			internal static void SwapBuffers() {
			internal static void SetViewport(Rect screenRect) {
			internal static void Cartesian2D(float scaleX = 1, float scaleY = 1, float offsetX = 0, float offsetY = 0) {
			internal static void ViewLookAt(Vector3 position, Vector3 target, Vector3 up) {
			internal static void ViewOrientation(Vector3 position, Quaternion rotation) {
			internal static void Perspective(float fovy, float aspect, float depthNear, float depthFar, float centerX = 0, float centerY = 0) {
			internal static void Orthographic(float width, float height, float depthNear, float depthFar, float centerX = 0, float centerY = 0) {
			internal static void SetProjection(Matrix4 matrix) {
			internal static void SetBackfaceCulling(bool onOrOff) {
			internal static void SetTransform(Matrix4 matrix) {
			internal static void SetDrawColor(float r, float g, float b, float a) {
			internal static void SetDrawColor(Color col) {
			internal static void StartStencillingWhileDrawing(bool inverseStencil = false) {
			internal static void StartStencillingWithoutDrawing(bool inverseStencil = false) {
			private static void StartStencilling(bool canDraw, bool inverseStencil) {
			internal static void StartUsingStencil() {
			internal static void LiftStencil() {
			private static bool disposed = false;
			public static void Dispose(bool disposing) {

				*/

		void nlineStartLineSegment(float x, float y, vec2 dir, vec2 perp);
		void nlineMoveLineSegmentInDirectionOf(float x, float y, bool useAverage, vec2 dir, vec2 perp);
	public:
		static Window* s_instance;

		void onResize(GLFWwindow* window, int width, int height);

		virtual void initialize() = 0;
		virtual void update() = 0;
		virtual void render() = 0;

		Window(int w, int h, const std::string& title);
		virtual ~Window();

		void run();
		void setState(WindowMode mode);
		void setSize(int w, int h);
		void setTitle(const std::string& title);
		SizeI getSize();


		// --- rendering functions

		void drawTriangle(Vertex v1, Vertex v2, Vertex v3);
		void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, float u0 = 0.0f, float v0 = 0.0f, float u1 = 0.5f, float v1 = 1.0f, float u2 = 1, float v2 = 0);
		void drawTriangleOutline(float thickness, float x0, float y0, float x1, float y1, float x2, float y2);

		void drawQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4);
		void drawQuad2D(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f, float u3 = 0.0f, float v3 = 1.0f);

		void drawRect(float x0, float y0, float x1, float y1, float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
		void drawRect(Rect rect, Rect uvs);
		void drawRect(Rect rect);
		void drawRectOutline(float thickness, Rect rect);
		void drawRectOutline(float thickness, float x0, float y0, float x1, float y1);

		void beginNGon2D(float centerX, float centerY, int n);
		void beginNGon(Vertex v1, int n);
		void continueNGon2D(float centerX, float centerY);
		void continueNGon(Vertex v);
		void endNGon();

		// TODO: fix shoddy implementation, possibly rewrite
		void beginNLine(float x, float y, float thickness, CapType cap);
		void continueNLine(float x, float y, bool useAverage = true);
		void endNLine(float x, float y);
		void disableEndingNLine();
		void enableEndingNLine();

		void drawArc(float xCenter, float yCenter, float radius, float startAngle, float endAngle);
		void drawArc(float xCenter, float yCenter, float radius, float startAngle, float endAngle, int edgeCount);
		void drawArcOutline(float thickness, float x0, float y0, float r, float startAngle, float endAngle);
		void drawArcOutline(float thickness, float xCenter, float yCenter, float radius, float startAngle, float endAngle, int edgeCount);


		void drawCircle(float x0, float y0, float r, int edges);
		void drawCircle(float x0, float y0, float r);
		void drawCircleOutline(float thickness, float x0, float y0, float r, int edges);
		void drawCircleOutline(float thickness, float x0, float y0, float r);


		void drawLine(float x0, float y0, float x1, float y1, float thickness, CapType cap);
		void drawLineOutline(float outlineThickness, float x0, float y0, float x1, float y1, float thickness, CapType cap);
		void drawLineCap(float x0, float y0, float radius, CapType cap, float startAngle);
		void drawLineCircleCap(float x0, float y0, float thickness, float angle);
		void drawLineCapOutline(float outlineThickness, float x0, float y0, float radius, CapType cap, float startAngle);
		void drawLineDefaultCapOutline(float thickness, float x0, float y0, float radius, float angle);
		void drawLineCircleCapOutline(float thickness, float x0, float y0, float radius, float angle);

		void setTextFont(std::string name, int size);
		float getTextWidth();
		float getTextHeight();
		float getTextWidth(char c);
		float getTextHeight(char c);
		vec2 getTextSize(char c);

		Texture getTextTexture();
		vec2 drawText(std::string text, float startX, float startY, HAlign hAlign, VAlign vAlign, float scale = 1.0f);
		vec2 drawText(std::string text, float startX, float startY, float scale = 1.0f);
		vec2 drawText(std::string text, int start, int end, float startX, float startY, float scale);
		float getTextStringHeight(std::string s);
		float getTextStringHeight(std::string s, int start, int end);
		float getTextStringWidth(std::string s);
	};
}

