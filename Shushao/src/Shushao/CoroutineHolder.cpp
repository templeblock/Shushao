#include "sepch.h"

#include "CoroutineHolder.h"

namespace se {

	CoroutineHolder::~CoroutineHolder() {
		StopAllCoroutines();
		ResumeCoroutines();
	}

	Coroutine* CoroutineHolder::StartCoroutine(Routine InRoutine) {
		RoutinePull Pull(InRoutine);
		if (Pull) {
			return PushCoroutine(new Coroutine(Pull));
		}
		return nullptr;
	}

	void CoroutineHolder::StopCoroutine(Coroutine* CoroutinePtr) {
		YieldInstruction* YieldReturnPtr = nullptr;
		while (CoroutinePtr != nullptr) {
			YieldReturnPtr = CoroutinePtr->GetYieldReturn();
			CoroutinePtr->End();
			CoroutinePtr = CastAsCoroutine(YieldReturnPtr);
		}

		if (YieldReturnPtr != nullptr) {
			YieldReturnPtr->End();
		}
	}

	void CoroutineHolder::StopAllCoroutines() {
		for (Coroutine* CoroutinePtr : Coroutines) {
			StopCoroutine(CoroutinePtr);
		}
	}

	bool CoroutineHolder::HasCoroutines() {
		return !Coroutines.empty();
	}

	Coroutine* CoroutineHolder::CastAsCoroutine(YieldInstruction* YieldInstructionPtr) {
		if (YieldInstructionPtr == nullptr || YieldInstructionPtr->Type != TYPEID(Coroutine)) {
			return nullptr;
		}
		return static_cast<Coroutine*>(YieldInstructionPtr);
	}

	bool CoroutineHolder::YieldCoroutine(Coroutine* CoroutinePtr) {
		YieldInstruction* YieldReturnPtr = CoroutinePtr->GetYieldReturn();
		if (YieldReturnPtr != nullptr) {
			if (YieldReturnPtr->IsEnded()) {
				delete YieldReturnPtr;
			}
			else {
				Coroutines.insert(CoroutinePtr);
				return true;
			}
		}
		return false;
	}

	Coroutine* CoroutineHolder::PushCoroutine(Coroutine* CoroutinePtr) {
		Coroutine* YieldReturnPtr = CastAsCoroutine(CoroutinePtr->GetYieldReturn());
		if (YieldReturnPtr == nullptr) {
			Coroutines.insert(CoroutinePtr);
		}
		else {
			YieldReturnPtr->NextPtr = CoroutinePtr;
		}
		return CoroutinePtr;
	}

	Coroutine* CoroutineHolder::PopCoroutine(Coroutine* CoroutinePtr) {
		Coroutine* NextPtr = CoroutinePtr->NextPtr;
		delete CoroutinePtr;
		return NextPtr;
	}

	void CoroutineHolder::ResumeCoroutines() {
		if (Coroutines.empty()) return;
		std::set<Coroutine*> CoroutinesCopy;
		CoroutinesCopy.swap(Coroutines);

		for (Coroutine* CoroutinePtr : CoroutinesCopy) {
			if (YieldCoroutine(CoroutinePtr)) {
				continue;
			}

			while (CoroutinePtr != nullptr) {
				CoroutinePtr->Resume();
				if (!CoroutinePtr->IsEnded()) {
					PushCoroutine(CoroutinePtr);
					break;
				}
				CoroutinePtr = PopCoroutine(CoroutinePtr);
			}
		}
	}

}  // namespace se