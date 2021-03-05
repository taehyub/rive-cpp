#ifndef _RIVE_STATE_MACHINE_LAYER_BASE_HPP_
#define _RIVE_STATE_MACHINE_LAYER_BASE_HPP_
#include "animation/state_machine_component.hpp"
namespace rive
{
	class StateMachineLayerBase : public StateMachineComponent
	{
	protected:
		typedef StateMachineComponent Super;

	public:
		static const int typeKey = 57;

		/// Helper to quickly determine if a core object extends another without
		/// RTTI at runtime.
		bool isTypeOf(int typeKey) const override
		{
			switch (typeKey)
			{
				case StateMachineLayerBase::typeKey:
				case StateMachineComponentBase::typeKey:
					return true;
				default:
					return false;
			}
		}

		int coreType() const override { return typeKey; }

	protected:
	};
} // namespace rive

#endif