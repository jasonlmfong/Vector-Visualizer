#include "ModeVectorTransformation.h"

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

namespace displayMode
{
    ModeVectorTransformation::ModeVectorTransformation()
    {
    }

    ModeVectorTransformation::ModeVectorTransformation(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;

        m_Translate = glm::vec3(0.0f);
        m_RotateAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        m_RotateAngle = 0.0f;
        m_Scale = glm::vec3(1.0f);

        m_Vectors->push_back(m_Vectors->at(0));
        AddVectorBufferData(*m_VectorBuffer, m_Vectors->at(0));
        CalculateMatrix();
    }

    ModeVectorTransformation::~ModeVectorTransformation()
    {
    }

    void ModeVectorTransformation::OnUpdate(float deltaTime)
    {
    }

    void ModeVectorTransformation::OnRender()
    {
    }

    void ModeVectorTransformation::OnImGuiRender()
    {
        // imgui matrix controls
        ImGui::Begin("Matrix Transformation");
        ImGui::Text("Matrix entries");
        ImGui::SliderFloat3("Translate", &m_Translate.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotation Axis", &m_RotateAxis.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Rotation Angle", &m_RotateAngle, 0.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &m_Scale.x, -10.0f, 10.0f);

        ImGui::Text("Transform Matrix");
        for (int i = 0; i < 4; i++)
        {
            ImGui::Text("%f %f %f %f", m_Transform[0][i], m_Transform[1][i], m_Transform[2][i], m_Transform[3][i]);
        }

        if (ImGui::Button("Update Matrix"))
        {
            CalculateMatrix();
            UpdateTransformedVector();
        }

        ImGui::End();
    }

    void ModeVectorTransformation::CalculateMatrix()
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, m_Scale);
        transform = glm::rotate(transform, glm::radians(m_RotateAngle), m_RotateAxis);
        transform = glm::translate(transform, m_Translate);

        m_Transform = transform;
    }

    void ModeVectorTransformation::UpdateTransformedVector()
    {
        VectorObject transformedVector;
        transformedVector.m_Origin = m_Transform * glm::vec4(m_Vectors->at(0).m_Origin, 1.0f);
        transformedVector.m_Direction = m_Transform * glm::vec4(m_Vectors->at(0).m_Direction, 1.0f);

        m_Vectors->at(1) = transformedVector;
        EditVectorBufferData(*m_VectorBuffer, *m_Vectors, 1);

        m_VectorVA->Bind();
        m_VectorVB->Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
    }

}