/*
 * Copyright (c) 2020, Stanislav Vorobiov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _OGL_H_
#define _OGL_H_

#include "af3d/Single.h"
#if defined(ANDROID) || defined(__ANDROID__)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY GL_APIENTRY
#endif

namespace af3d
{
    class OGL : public Single<OGL>
    {
    public:
        OGL();
        ~OGL();

        void (GLAPIENTRY* DrawBuffers)(GLsizei n, const GLenum* bufs);
        void (GLAPIENTRY* GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
        void (GLAPIENTRY* GenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
        void (GLAPIENTRY* DeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
        void (GLAPIENTRY* BindRenderbuffer)(GLenum target, GLuint renderbuffer);
        void (GLAPIENTRY* RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
        void (GLAPIENTRY* FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
        void (GLAPIENTRY* PixelStorei)(GLenum pname, GLint param);
        void (GLAPIENTRY* GenSamplers)(GLsizei n, GLuint* samplers);
        void (GLAPIENTRY* DeleteSamplers)(GLsizei n, const GLuint* samplers);
        void (GLAPIENTRY* BindSampler)(GLuint unit, GLuint sampler);
        void (GLAPIENTRY* SamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
        void (GLAPIENTRY* SamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
        void (GLAPIENTRY* DrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
        void (GLAPIENTRY* DepthMask)(GLboolean flag);
        void (GLAPIENTRY* DepthFunc)(GLenum func);
        void (GLAPIENTRY* CullFace)(GLenum mode);
        void (GLAPIENTRY* DrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
        void (GLAPIENTRY* GenVertexArrays)(GLsizei n, GLuint* array);
        void (GLAPIENTRY* DeleteVertexArrays)(GLsizei n, const GLuint* arrays);
        void (GLAPIENTRY* BindVertexArray)(GLuint array);
        void (GLAPIENTRY* GenBuffers)(GLsizei n, GLuint* buffers);
        void (GLAPIENTRY* DeleteBuffers)(GLsizei n, const GLuint* buffers);
        void (GLAPIENTRY* BindBuffer)(GLenum target, GLuint buffer);
        void (GLAPIENTRY* BufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
        void (GLAPIENTRY* BufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
        void* (GLAPIENTRY* MapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
        GLboolean (GLAPIENTRY* UnmapBuffer)(GLenum target);
        void (GLAPIENTRY* GenTextures)(GLsizei n, GLuint* textures);
        void (GLAPIENTRY* DeleteTextures)(GLsizei n, const GLuint* textures);
        void (GLAPIENTRY* BindTexture)(GLenum target, GLuint texture);
        void (GLAPIENTRY* ActiveTexture)(GLenum texture);
        void (GLAPIENTRY* TexImage2D)(GLenum target, GLint level,
            GLint internalFormat,
            GLsizei width, GLsizei height,
            GLint border, GLenum format, GLenum type,
            const GLvoid* pixels);
        void (GLAPIENTRY* TexImage3D)(GLenum target,
            GLint level,
            GLint internalformat,
            GLsizei width,
            GLsizei height,
            GLsizei depth,
            GLint border,
            GLenum format,
            GLenum type,
            const void* data);
        void (GLAPIENTRY* CompressedTexImage2D)(GLenum target,
            GLint level,
            GLenum internalformat,
            GLsizei width,
            GLsizei height,
            GLint border,
            GLsizei imageSize,
            const void * data);
        void (GLAPIENTRY* TexSubImage3D)(GLenum target,
            GLint level,
            GLint xoffset,
            GLint yoffset,
            GLint zoffset,
            GLsizei width,
            GLsizei height,
            GLsizei depth,
            GLenum format,
            GLenum type,
            const void* data);
        void (GLAPIENTRY* TexParameteri)(GLenum target, GLenum pname, GLint param);
        void (GLAPIENTRY* ClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
        void (GLAPIENTRY* Clear)(GLbitfield mask);
        void (GLAPIENTRY* Viewport)(GLint x, GLint y,
                                    GLsizei width, GLsizei height);
        void (GLAPIENTRY* AttachShader)(GLuint program, GLuint shader);
        void (GLAPIENTRY* BindAttribLocation)(GLuint program, GLuint index, const GLchar* name);
        void (GLAPIENTRY* CompileShader)(GLuint shader);
        GLuint (GLAPIENTRY* CreateProgram)();
        GLuint (GLAPIENTRY* CreateShader)(GLenum type);
        void (GLAPIENTRY* DeleteProgram)(GLuint program);
        void (GLAPIENTRY* DeleteShader)(GLuint shader);
        void (GLAPIENTRY* DetachShader)(GLuint program, GLuint shader);
        void (GLAPIENTRY* DisableVertexAttribArray)(GLuint index);
        void (GLAPIENTRY* EnableVertexAttribArray)(GLuint index);
        void (GLAPIENTRY* LinkProgram)(GLuint program);
        void (GLAPIENTRY* ShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
        void (GLAPIENTRY* UseProgram)(GLuint program);
        void (GLAPIENTRY* GetProgramiv)(GLuint program, GLenum pname, GLint* params);
        void (GLAPIENTRY* GetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
        void (GLAPIENTRY* GetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
        void (GLAPIENTRY* GetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
        void (GLAPIENTRY* GetShaderiv)(GLuint shader, GLenum pname, GLint* params);
        void (GLAPIENTRY* GetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
        GLint (GLAPIENTRY* GetAttribLocation)(GLuint program, const GLchar* name);
        GLint (GLAPIENTRY* GetUniformLocation)(GLuint program, const GLchar* name);
        void (GLAPIENTRY* VertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
        void (GLAPIENTRY* DrawArrays)(GLenum mode, GLint first, GLsizei count);
        void (GLAPIENTRY* Uniform1iv)(GLint location, GLsizei count, const GLint* value);
        void (GLAPIENTRY* Uniform1fv)(GLint location, GLsizei count, const GLfloat* value);
        void (GLAPIENTRY* Uniform2fv)(GLint location, GLsizei count, const GLfloat* value);
        void (GLAPIENTRY* Uniform3fv)(GLint location, GLsizei count, const GLfloat* value);
        void (GLAPIENTRY* Uniform4fv)(GLint location, GLsizei count, const GLfloat* value);
        void (GLAPIENTRY* Enable)(GLenum cap);
        void (GLAPIENTRY* Disable)(GLenum cap);
        void (GLAPIENTRY* BlendFunc)(GLenum sfactor, GLenum dfactor);
        void (GLAPIENTRY* BlendFuncSeparate)(GLenum sfactor, GLenum dfactor, GLenum srcAlpha, GLenum dstAlpha);
        void (GLAPIENTRY* UniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
        void (GLAPIENTRY* UniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
        void (GLAPIENTRY* Uniform1i)(GLint location, GLint v0);
        void (GLAPIENTRY* Uniform2i)(GLint location, GLint v0, GLint v1);
        void (GLAPIENTRY* GetIntegerv)(GLenum pname, GLint* params);
        void (GLAPIENTRY* GenerateMipmap)(GLenum target);
        void (GLAPIENTRY* PointSize)(GLfloat size);
        void (GLAPIENTRY* LineWidth)(GLfloat width);
        void (GLAPIENTRY* GenFramebuffers)(GLsizei n, GLuint* framebuffers);
        void (GLAPIENTRY* DeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
        void (GLAPIENTRY* BindFramebuffer)(GLenum target, GLuint framebuffer);
        void (GLAPIENTRY* FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
        void (GLAPIENTRY* FramebufferTextureLayer)(GLenum target, GLenum attachment,
            GLuint texture,
            GLint level,
            GLint layer);
        GLenum (GLAPIENTRY* CheckFramebufferStatus)(GLenum target);
        void (GLAPIENTRY* Uniform1f)(GLint location, GLfloat v0);
        void (GLAPIENTRY* Uniform2f)(GLint location, GLfloat v0, GLfloat v1);
        void (GLAPIENTRY* ColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
        void (GLAPIENTRY* StencilFunc)(GLenum func, GLint ref, GLuint mask);
        void (GLAPIENTRY* StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
        const GLubyte* (GLAPIENTRY* GetString)(GLenum name);
        const GLubyte* (GLAPIENTRY* GetStringi)(GLenum name, GLuint index);
        void (GLAPIENTRY* Scissor)(GLint x, GLint y, GLsizei width, GLsizei height);
        void (GLAPIENTRY* GetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint* params);
        void (GLAPIENTRY* GetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei* length, GLint* params);
        void (GLAPIENTRY* GetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, char* name);
        void (GLAPIENTRY* BindBufferBase)(GLenum target, GLuint index, GLuint buffer);
        void (GLAPIENTRY* DispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
        void (GLAPIENTRY* MemoryBarrier)(GLbitfield barriers);
    };

    extern OGL ogl;
}

#endif
