#pragma once

#include "RawModel.h"
#include "WaterShader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"
#include "Camera.h"

class WaterRenderer
{
private:
	RawModel m_quad;
	WaterShader m_shader;
	WaterFrameBuffers& m_fbos;
public:
	WaterRenderer(WaterFrameBuffers& fbos) : m_quad(RawModel({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f }, 2)), m_fbos(fbos)
	{ }

	void render(const std::vector<WaterTile>& water, const Camera& camera)
	{
		prepareRender(camera);
		for (const WaterTile& tile : water)
		{
			m_shader.setModelMatrix(tile.getModelMatrix());
			glDrawArrays(GL_TRIANGLES, 0, m_quad.getVertexCount());
		}
		unbind();
	}

	void prepareRender(const Camera& camera)
	{
		m_shader.use();
		m_shader.setViewMatrix(camera);
		m_shader.setProjectionMatrix(camera);
		m_quad.bind();
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbos.getReflectionTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_fbos.getRefractionTexture());
	}

	void unbind()
	{
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_shader.stop();
	}
};