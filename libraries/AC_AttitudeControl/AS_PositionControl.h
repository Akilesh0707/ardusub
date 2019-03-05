
#include <AP_AHRS/AP_AHRS.h>
//#include <AP_Baro/AP_Baro.h>
//#include <stdio.h>

class AS_PositionControl
{

public:
    AS_PositionControl(AP_AHRS& ahrs) : ahrs_(ahrs)
    {
    }

    void set_target_position(Vector3f target) { target_position = target; }
    void set_target_velocity(Vector3f target) { target_velocity = target; }
    void set_target_acceleration(Vector3f target) { target_acceleration = target; }

    Vector3f get_target_position() { return target_position; }
    Vector3f get_target_velocity() { return target_velocity; }
    Vector3f get_target_acceleration() { return target_acceleration; }

    Vector3f get_error_position() { return target_position - get_current_position(); }
    Vector3f get_error_velocity() { return target_velocity - get_current_velocity(); }
    Vector3f get_error_acceleration() { return target_acceleration - get_current_acceleration(); }

    Vector3f get_current_position() { return Vector3f { 0, 0, ahrs_.get_baro().get_altitude() }; }
    //return ahrs_.get_relative_position_NED_origin();
    Vector3f get_current_velocity() { Vector3f a; ahrs_.get_velocity_NED(a); return a;}
    Vector3f get_current_acceleration() { return ahrs_.get_accel_ef_blended(); }

    void update()
    {
        update_position_controller();
        update_velocity_controller();
        update_acceleration_controller();
    };

    Vector3f get_output_command();

private:
    AC_PID pid_position_x { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };
    AC_PID pid_velocity_x { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };
    AC_PID pid_acceleration_x { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };

    AC_PID pid_position_y { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };
    AC_PID pid_velocity_y { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };
    AC_PID pid_acceleration_y { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };

    AC_PID pid_position_z { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };
    AC_PID pid_velocity_z { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };
    AC_PID pid_acceleration_z { 1.0f, 1.0f, 1.0f, 100.0f, 100.0f, 0.01f };

    // meters!
    Vector3f target_position = { 0, 0, 0 };
    Vector3f target_velocity = { 0, 0, 0 };
    Vector3f target_acceleration = { 0, 0, 0 };
    Vector3f output_command = { 0, 0, 0 };

    const AP_AHRS& ahrs_;

    void update_position_controller()
    {
        pid_position_z.set_input_filter_all(get_error_position().z);
        target_velocity.z = pid_position_z.get_pid();
    }

    void update_velocity_controller()
    {
        pid_velocity_z.set_input_filter_all(get_error_velocity().z);
        target_acceleration.z = pid_velocity_z.get_pid();
    }

    void update_acceleration_controller()
    {
        pid_acceleration_z.set_input_filter_all(get_error_acceleration().z);
        output_command.z = pid_acceleration_z.get_pid();
    }
};
