#pragma once

#include "globals.h"

class Mover : public se::Program {
public:
protected:
    void FixedUpdate();
    void Update();
    void Awake();
    void Start();

    void OnCollisionEnter2D(se::Collision2D&);
    void OnCollisionExit2D(se::Collision2D&);
    void OnCollisionStay2D(se::Collision2D&);

    void OnTriggerEnter2D(se::Collider2D&);
    void OnTriggerExit2D(se::Collider2D&);
    void OnTriggerStay2D(se::Collider2D&);

private:
    float animTime;
    se::SpriteRenderer* sr = nullptr;
    se::Rigidbody2D* rb = nullptr;
};
