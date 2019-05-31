#include "pancrazio.h"
#include "mover.h"

using namespace se;

void Pancrazio::Awake() {
    sr = AddComponent<SpriteRenderer>();
    sr->sprite = Resources::Get<Sprite>("pancrazio_sprite");
    sr->sortingLayerID = Config::SortingLayers["Characters"];

    rb = AddComponent<Rigidbody2D>();
    //rb->SetType(RigidbodyType::KINEMATIC);
    //rb->setFixedRotation(true);
    collider = AddComponent<BoxCollider2D>("box");
    collider->SetShape({0.5f, 0.5f});

    BoxCollider2D* trigger = AddComponent<BoxCollider2D>("trigger");
    trigger->SetSensor(true);
    trigger->SetShape({1.5f, 0.1f});

    //circlec = AddComponent<CircleCollider2D>("circle");
    //circlec->SetShape({0.0f, 0.0f}, 1.0f);

    /*animation = AddComponent<Animation>("walk");
	animation->setFPS(1);
	animation->setLoop(true);
	animation->addLayer("sprite")->setTarget(sr->sprite)->load(Resources::Get<SpriteSheet>("walking"));
	animation->addLayer("size")->setTarget(transform->localScale)->load({
		{1.0f, 1.0f, 0.0f},
		{1.1f, 1.1f, 0.0f},
		{1.2f, 1.2f, 0.0f},
		{1.3f, 1.3f, 0.0f}
	});

	animation->setStartState(AnimationState::PLAY);
	transform->localScale = {1.0f, 1.0f, 0.0f};*/
}
