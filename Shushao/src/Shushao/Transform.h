#ifndef SE_TRANSFORM
#define SE_TRANSFORM

#include "Core.h"
#include "sepch.h"

#include "Component.h"
#include "Rect.h"
#include "recttransform.h"

namespace se {

class Transform : public Component {
public:
    enum class Origin {
        LOCAL,
        WORLD
    };

    ~Transform();

    virtual void setup();

    bool isRoot = false;
    bool isFree = false;

    Origin origin = Origin::LOCAL;

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 localPosition = VEC3_ZERO;
    glm::quat localRotation = QUATERNION_IDENTITY;
    glm::vec3 localScale = VEC3_IDENTITY;

    // readonly properties
    const glm::vec3& position = _position;
    const glm::quat& rotation = _rotation;
    const glm::vec3& scale = _scale;
    const glm::vec3& pivot = _pivot;
    const glm::mat4& MV = _MV;
    const glm::mat4& VP = _VP;
    const glm::mat4& MVP = _MVP;
    const glm::mat4& M = _M;
    const glm::mat4& V = _V;
    const glm::mat4& P = _P;
    const glm::vec3& forward = _forward;
    const glm::vec3& right = _right;
    const glm::vec3& up = _up;
    const glm::vec3& velocity = _velocity;

    const glm::mat4& localToWorldMatrix = _localToWorldMatrix;
    const glm::mat4& localToParentMatrix = _localToParentMatrix;
    const glm::mat4& worldToLocalMatrix = _worldToLocalMatrix;
    const glm::mat4& rootMatrix = _rootMatrix;

    // methods
    bool isAtRoot();
    Transform* GetParent();
    Transform* GetChild(unsigned int);
    std::vector<Transform*> GetParents();
    void SetParent(Transform*, bool worldPositionStays = false);
    void RemoveChild(Transform* t);
    void AddChild(Transform* t);

    float* uMVP();  // MVP Matrix for uniforms
    float* uMV();  // Model-View Matrix for uniforms
    float* uVP();  // Model-View Matrix for uniforms
    float* uM();  // Model Matrix for uniforms
    float* uP();  // Projection Matrix for uniforms
    float* uV();  // View Matrix for uniforms

    glm::mat4 GetLocalToParentMatrix();
    glm::mat4 GetLocalToWorldMatrix();
    glm::mat4 GetWorldToLocalMatrix();
    glm::mat4 GetRootMatrix();
    glm::vec3 GetWorldPosition();
    glm::vec3 GetLocalPosition(glm::vec3);
    glm::vec3 GetWorldScale();
    glm::quat GetWorldOrientation();

    // lifecycle
    void Awake();
    void Update();
    void Render();

    //{ #region getter and setters

    glm::vec3 GetEulerAngles();
    glm::vec3 GetLocalEulerAngles();

    void SetPivot(glm::vec2);
    void SetPosition(glm::vec3);
    void SetLocalPosition(glm::vec3);
    void SetLocalRotation(glm::quat);
    void SetLocalRotation(glm::vec3);
    void SetLocalScale(glm::vec3);
    void SetRotation(glm::quat);
    void SetRotation(glm::vec3);

    //}

    static const glm::mat4 MAT4_IDENTITY;
    static const glm::vec3 VEC3_ZERO;
    static const glm::vec3 VEC3_IDENTITY;
    static const glm::vec3 VEC3_IDENTITY2D;
    static const glm::vec3 UP;
    static const glm::vec3 DOWN;
    static const glm::vec3 FORWARD;
    static const glm::vec3 BACK;
    static const glm::vec3 RIGHT;
    static const glm::vec3 LEFT;
    static const glm::quat QUATERNION_IDENTITY;
    static const glm::vec3 AXIS_X;
    static const glm::vec3 AXIS_Y;
    static const glm::vec3 AXIS_Z;

    void Invalidate();

    RectTransform* rectTransform = nullptr;
    bool isRectTransform = false;

    void Copy(Transform* other);

private:
    bool matrixInvalid = false;
    bool inverseMatrixInvalid = false;

    glm::vec3 _position = VEC3_ZERO;
    glm::quat _rotation = QUATERNION_IDENTITY;
    glm::vec3 _scale = VEC3_IDENTITY2D;
    glm::vec3 _velocity = VEC3_ZERO;
    glm::vec3 _forward = FORWARD;
    glm::vec3 _right = RIGHT;
    glm::vec3 _up = UP;
    glm::vec3 _pivot = VEC3_ZERO;
    glm::mat4 _MVP = glm::mat4();
    glm::mat4 _MV = glm::mat4();
    glm::mat4 _VP = glm::mat4();
    glm::mat4 _M = glm::mat4();
    glm::mat4 _V = glm::mat4();
    glm::mat4 _P = glm::mat4();

    glm::mat4 _localToWorldMatrix = glm::mat4();
    glm::mat4 _localToParentMatrix = glm::mat4();
    glm::mat4 _worldToLocalMatrix = glm::mat4();
    glm::mat4 _rootMatrix = glm::mat4();

    glm::vec3 lastPosition = VEC3_ZERO;

    void setupDirections();
    void buildMVP();

    void updateRectTransforms();
};

}  // namespace se

#endif