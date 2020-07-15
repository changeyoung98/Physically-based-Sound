using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class coliisionInfo : MonoBehaviour
{
    private Vector3 _prevVelocity;
    private bool _setPrevVelocity = true;

    private Vector3 _prevPosition;
    private Vector3 _kinematicVelocity;
    private Quaternion _prevRotation;
    private float _kinematicAngularVelocity;

    private int _lastFrame;

    private Rigidbody _r;
    private Rigidbody2D _r2D;
    // Start is called before the first frame update
    void Start()
    {
        _r = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        _kinematicVelocity = (transform.position - _prevPosition) / Time.deltaTime;
        _prevPosition = transform.position;

        _kinematicAngularVelocity = Quaternion.Angle(_prevRotation, transform.rotation) / Time.deltaTime / 45f;
        _prevRotation = transform.rotation;
    }

    #region 3D Collision Messages

    Vector3 contactNormal, contactPoint, relativeVelocity;

    void OnCollisionEnter(Collision c)
    {
        //if (SoundMaterial == null || !this.enabled || SoundMaterial.AudioSets.Count == 0)
        //    return;

        contactNormal = c.contacts[0].normal;
        contactPoint = c.contacts[0].point;
        relativeVelocity = c.relativeVelocity;

    //    playImpactSound(c.collider.gameObject, relativeVelocity, contactNormal, contactPoint);

        _setPrevVelocity = true;
        Vector3 force = relativeVelocity * _r.mass / Time.deltaTime;
        Debug.Log(force);
    }


    void OnCollisionStay(Collision c)
    {
       // if (SoundMaterial == null || !this.enabled || SoundMaterial.AudioSets.Count == 0 || _audioContainersDic == null)
       //     return;

        if (_setPrevVelocity)
        {
            _prevVelocity = _r.velocity;
            _setPrevVelocity = false;
        }

        Vector3 deltaVel = _r.velocity - _prevVelocity;

        contactNormal = c.contacts[0].normal;
        contactPoint = c.contacts[0].point;
        relativeVelocity = c.relativeVelocity;

    //    playImpactSound(c.collider.gameObject, deltaVel, contactNormal, contactPoint);
    //    setSlideTargetVolumes(c.collider.gameObject, relativeVelocity, contactNormal, contactPoint, false);

        _prevVelocity = _r.velocity;
    }

    void OnCollisionExit()
    {
       // if (SoundMaterial == null || !this.enabled || SoundMaterial.AudioSets.Count == 0 || _audioContainersDic == null)
       //     return;

    //    setSlideTargetVolumes(null, Vector3.zero, Vector3.zero, transform.position, true);
        _setPrevVelocity = true;
    }

    #endregion
}
