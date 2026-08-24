#pragma once

#ifndef QT_NO_OPENGL_HEADERS
#	define QT_NO_OPENGL_HEADERS
#endif

#include <glad/glad.h>

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

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QSurface>
#include <QSurfaceFormat>