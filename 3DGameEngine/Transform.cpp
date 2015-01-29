#include "transform.h"

Camera *Transform::m_camera;
Matrix4f Transform::projectionMatrix;
/*float Transform::m_zFar;
float Transform::m_zNear;
float Transform::m_fov;
float Transform::m_aspect;*/

Transform::Transform(Vector3f pos, Quaternion orientation, Vector3f scale)
{
	m_pos = pos;
	m_orientation = orientation;
	m_scale = scale;
}

Transform::~Transform()
{

}

Matrix4f Transform::getTransformation()
{
	Matrix4f translationMatrix;
	Matrix4f rotationMatrix;
	Matrix4f scaleMatrix;

	translationMatrix.initTranslationTransform(m_pos.getX(), m_pos.getY(), m_pos.getZ());
	rotationMatrix.initRotateTransform(m_orientation.getX(), m_orientation.getY(), m_orientation.getZ(), m_orientation.getW());
	scaleMatrix.initScaleTransform(m_scale.getX(), m_scale.getY(), m_scale.getZ());

	return translationMatrix * rotationMatrix * scaleMatrix;
}

Matrix4f Transform::getProjectedTransformation()
{
	Matrix4f transformationMatrix = getTransformation();
	Matrix4f cameraRotation;
	Matrix4f cameraTranslation;

	cameraRotation.initCameraTransform(m_camera->getForward(), m_camera->getUp());
	cameraTranslation.initTranslationTransform(-m_camera->getPos().getX(), -m_camera->getPos().getY(), -m_camera->getPos().getZ());

	Matrix4f viewMatrix = cameraRotation * cameraTranslation;

	return projectionMatrix * viewMatrix * transformationMatrix; // Model-View-Projection matrix (MVP)
}

Vector3f& Transform::getPos()
{
	return m_pos;
}

Quaternion& Transform::getOrientation()
{
	return m_orientation;
}

Vector3f& Transform::getScale()
{
	return m_scale;
}

void Transform::setPos(Vector3f& pos)
{
	this->m_pos = pos;
}

void Transform::setOrientation(Quaternion& orientation)
{
	this->m_orientation = orientation;
}

void Transform::setScale(Vector3f& scale)
{
	this->m_scale = scale;
}

void Transform::setProjectionMatrix(float fov, float aspect, float zNear, float zFar)
{
	projectionMatrix.initPersProjTransform(fov, aspect, zNear, zFar);
}

Matrix4f Transform::getProjectionMatrix()
{
	return projectionMatrix;
}

Camera *Transform::getCamera()
{
	return m_camera;
}

void Transform::setCamera(Camera *camera)
{
	Transform::m_camera = camera;
}