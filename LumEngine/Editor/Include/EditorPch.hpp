#pragma once

#ifndef QT_NO_OPENGL_HEADERS
#   define QT_NO_OPENGL_HEADERS
#endif

#ifndef QT_NO_OPENGL
#   define QT_NO_OPENGL
#endif

#ifndef GLFW_INCLUDE_NONE
#   define GLFW_INCLUDE_NONE
#endif

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QImage>
#include <QCursor>
#include <QWindow>
#include <QVBoxLayout>

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QSurface>
#include <QSurfaceFormat>

#include <glad/glad.h>