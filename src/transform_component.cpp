#include "transform_component.hpp"
#include "shapes/clipping_shape.hpp"

using namespace rive;

StatusCode TransformComponent::onAddedClean(CoreContext* context)
{
	m_ParentTransformComponent =
	    parent() != nullptr && parent()->is<TransformComponent>()
	        ? parent()->as<TransformComponent>()
	        : nullptr;
	return StatusCode::Ok;
}

void TransformComponent::buildDependencies()
{
	if (parent() != nullptr)
	{
		parent()->addDependent(this);
	}
}

void TransformComponent::markTransformDirty()
{
	if (!addDirt(ComponentDirt::Transform))
	{
		return;
	}
	markWorldTransformDirty();
}

void TransformComponent::markWorldTransformDirty()
{
	addDirt(ComponentDirt::WorldTransform, true);
}

void TransformComponent::updateTransform()
{
	if (rotation() != 0)
	{
		Mat2D::fromRotation(m_Transform, rotation());
	}
	else
	{
		Mat2D::identity(m_Transform);
	}
	m_Transform[4] = x();
	m_Transform[5] = y();
	Mat2D::scaleByValues(m_Transform, scaleX(), scaleY());
}

void TransformComponent::updateWorldTransform()
{
	if (m_ParentTransformComponent != nullptr)
	{
		Mat2D::multiply(m_WorldTransform,
		                m_ParentTransformComponent->m_WorldTransform,
		                m_Transform);
	}
	else
	{
		Mat2D::copy(m_WorldTransform, m_Transform);
	}
}

void TransformComponent::update(ComponentDirt value)
{
	if (hasDirt(value, ComponentDirt::Transform))
	{
		updateTransform();
	}
	if (hasDirt(value, ComponentDirt::WorldTransform))
	{
		updateWorldTransform();
	}
	if (hasDirt(value, ComponentDirt::RenderOpacity))
	{
		m_RenderOpacity = opacity();
		if (m_ParentTransformComponent != nullptr)
		{
			m_RenderOpacity *= m_ParentTransformComponent->childOpacity();
		}
	}
}

const Mat2D& TransformComponent::transform() const { return m_Transform; }
const Mat2D& TransformComponent::worldTransform() const
{
	return m_WorldTransform;
}

void TransformComponent::rotationChanged() { markTransformDirty(); }
void TransformComponent::scaleXChanged() { markTransformDirty(); }
void TransformComponent::scaleYChanged() { markTransformDirty(); }
void TransformComponent::opacityChanged()
{
	addDirt(ComponentDirt::RenderOpacity, true);
}