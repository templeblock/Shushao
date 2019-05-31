#include "mover.h"

using namespace se;

float frand(float max) {
    return (float(rand()) / RAND_MAX) * max;
}

void Mover::Awake() {
    sr = entity->GetComponent<SpriteRenderer>();
    rb = entity->GetComponent<Rigidbody2D>();
}

void Mover::Start() {
    //
}

void Mover::Update() {
    /*float rotationSpeed = 0.2f;

	transform->localPosition.x += 1.5f * Time::deltaTime;

	//Transform* son = transform->getChild(0);

	transform->setLocalRotation({0,0, transform->getLocalEulerAngles().z + Time::deltaTime * rotationSpeed});

	if (transform->localPosition.x > 8.0f) {
		if (entity->GetComponent<Animation>("walk") != nullptr)
			entity->GetComponent<Animation>("walk")->stop();
		Disable();
	}*/
}

void Mover::FixedUpdate() {
#if 0
    double horizontal = Input::getAxis("horizontal");
    if (Input::getAxis("horizontal") != 0) {
        rb->body->ApplyForceToCenter({(float)horizontal * 10.0f, 0.0f}, true);
    }
#endif
}

void Mover::OnCollisionEnter2D(Collision2D& collision) {
    Debug::Log << "COLLISION: " << collision.collider->entity->name << " --> " << collision.otherCollider->entity->name << std::endl;
    Resources::Get<Effect>("hit")->play();
}

void Mover::OnCollisionExit2D(Collision2D& collision) {
    Debug::Log << "COLLISION EXIT: " << collision.collider->entity->name << " --> " << collision.otherCollider->entity->name << std::endl;
}

void Mover::OnCollisionStay2D(Collision2D& collision) {
    //if (collision.otherCollider->entity->name == "Door") std::cout << "COLLISION STAY!!!" << std::endl;
}

void Mover::OnTriggerEnter2D(Collider2D& other) {
    if (other.entity->name == "Door") Debug::Log << "TRIGGER ENTER DOOR" << std::endl;
}

void Mover::OnTriggerExit2D(Collider2D& other) {
    if (other.entity->name == "Door") Debug::Log << "TRIGGER EXIT DOOR" << std::endl;
}

void Mover::OnTriggerStay2D(Collider2D& other) {
    if (other.entity->name == "Door") Debug::Log << "TRIGGER STAY DOOR" << std::endl;
}