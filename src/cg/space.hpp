#pragma once

#include "_cgControl.hpp"

namespace CG {
    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> ortho(
        T left                        , T right                       ,
        T down                        , T up                          ,
        T zNear = static_cast<T>(+0.1), T zFar  = static_cast<T>(+1.0)
    ) {
        return {
            +(2 / (right - left)), 0                 , 0                    , -((right + left) / (right - left)),
            0                    , +(2 / (up - down)), 0                    , -((up + down) / (up - down))      ,
            0                    , 0                 , -(2 / (zFar - zNear)), -((zFar + zNear) / (zFar - zNear)),
            0                    , 0                 , 0                    , 1
        };
    }

    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> perspective(
        T left                        , T right                       ,
        T down                        , T up                          ,
        T zNear = static_cast<T>(+0.1), T zFar  = static_cast<T>(+1.0)
    ) {
        return {
            ((2.0f * zNear) / (right - left)), 0                             , ((right + left) / (right - left)) , 0                                    ,
            0                                , ((2.0f * zNear) / (up - down)), ((up + down) / (up - down))       , 0                                    ,
            0                                , 0                             , ((zFar + zNear) / (zFar - zNear)) , ((zFar * zNear * 2) / (zFar - zNear)),
            0                                , 0                             , -1                                , 0
        };
    }

    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> perspective(
        T fov, T aspectRatio, T zNear = static_cast<T>(+0.1), T zFar = static_cast<T>(+1.0), bool fovVertical = false
    ) {
        T right, up;
        if (fovVertical) {
            up = (zNear * gcem::tan(fov / 2.0f));
            right = (up * aspectRatio);
        }
        else {
            right = (zNear * gcem::tan(fov / 2.0f));
            up = (right / aspectRatio);
        }

        return CG::perspective<T>(-right, +right, -up, +up, zNear, zFar);
    }

    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> perspectiveDeg(
        T fovDeg, T aspectRatio, T zNear = static_cast<T>(+0.1), T zFar = static_cast<T>(+1.0), bool fovVertical = false
    ) {
        return CG::perspective<T>((fovDeg * degToRad<T>), aspectRatio, zNear, zFar, fovVertical);
    }

    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> model(
        const Vector<T, 3>  &position,
        const Quaternion<T> &rotation,
        const Vector<T, 3>  &scale
    ) {
        const Vector<T, 3> xAxis = ((rotation * Vector3::right  ) * scale.x);
        const Vector<T, 3> yAxis = ((rotation * Vector3::up     ) * scale.y);
        const Vector<T, 3> zAxis = ((rotation * Vector3::forward) * scale.z);

        return {
            xAxis.x          , yAxis.x          , zAxis.x          , position.x       ,
            xAxis.y          , yAxis.y          , zAxis.y          , position.y       ,
            xAxis.z          , yAxis.z          , zAxis.z          , position.z       ,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
        };
    }

    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> modelInverse(
        const Vector<T, 3>  &position,
        const Quaternion<T> &rotation,
        const Vector<T, 3>  &scale
    ) {
        return CG::model<T>(
            -position,
            rotation.inverse(),
            {((scale.x == 0)? 0:(1 / scale.x)), ((scale.y == 0)? 0:(1 / scale.y)), ((scale.z == 0)? 0:(1 / scale.z))});
    }

    template<std::floating_point T>
    inline constexpr Matrix<T, 4, 4> view(
        const Vector<T, 3>  &cameraPosition,
        const Quaternion<T> &cameraRotation,
        const Vector<T, 3>  &cameraScale
    ) {
        const auto rotation = CG::model<T>(Vector<T, 3>::zero, cameraRotation, Vector<T, 3>::one);

        const auto positionScale = CG::model(
            -cameraPosition,
            Quaternion<T>::identity,
            {((cameraScale.x == 0)? 0:(1 / cameraScale.x)), ((cameraScale.y == 0)? 0:(1 / cameraScale.y)), ((cameraScale.z == 0)? 0:(1 / cameraScale.z))});

        return (rotation * positionScale);
    }
}