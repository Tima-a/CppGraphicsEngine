//Air Drag Table Thanks to https://aerodyn.org/tables and https://en.wikipedia.org/wiki/Drag_coefficient
#define SphereAirDrag 0.47f
#define HalfSphere 0.42f
#define ConeAirDrag 0.50f
#define CubeAirDrag 1.05f
#define AngledCubeAirDrag 0.80f
#define LongCylinder 0.82f
#define ShortCylinderAirDrag 1.15f
#define StreamlinedBodyAirDrag 0.04f
#define StreamlinedHalfBodyAirDrag 0.09f

#define BulletAirDrag 0.295f
#define SkierAirDrag 1.05f
#define WireAirDrag 1.15f
#define AdultHumanAirDrag 1.15f
#define EmpireStateBuildingAirDrag 1.4f
#define EiffelTowerAirDrag 1.9f

#define F4Phantom2AirDrag 0.021f
#define Learjet24AirDrag 0.022f
#define Boeing787AirDrag 0.024f
#define AirBusA380AirDrag 0.0265f
#define Cessna172AirDrag 0.027f
#define Cessna182AirDrag 0.027f
#define Cessna310AirDrag 0.027f
#define Boeing747AirDrag 0.031f
#define F104StarFighterAirDrag 0.044f

#define HelicopterAirDrag 0.8f
#define SportsCarAirDrag 0.035f
#define TruckAirDrag 0.5f
#define VWPoloClassAAirDrag 0.37f
#define FordEscortClassBAirDrag 0.36f
#define OpenVectraClassCAirDrag 0.29f
#define BMW520iClassDAirDrag 0.31f
#define Mercedes300SEClassEAirDrag 0.36f

#define RoughSphereAirDrag 0.40f
#define SmoothSphereAirDrag 0.10f
#define HollowSemiSphereOppositeStream 1.42f
#define HollowSemiSphereFacingStream 0.38f
#define HollowSemiCylinderOppositeStream 1.20f
#define HollowSemiCylinderFacingStream 2.30f
#define SquaredFlatPlate90deg 1.17f
#define LongFlatPlate90deg 1.98f
#define OpenWheelRotating 0.58f

#define UniversalGasConstantForAir 287.052874f
#define UniversalGasConstant 8.31432f
#define StandardTemperatureLapseRate -0.0065f
#define EarthMolarMass 0.0289644f
#define StaticPressure 101325.0f
#define StandardTemperature 288.15f
#define GravitationalConstant 6.6743e-11
#define EarthMass 5.972e+24
#define MolarMassOfDryAir  0.0289652f
#define MolarMassOfHumidAir 0.018016f
#define EarthRadius 6371000
const float g_earth = -9.80665f;
float pixel_meter_ratio = 30.0f;
static enum AirResistanceType_
{
	NO_AR,
	AR_DEFAULT,
	AR_USER_INPUT
};
static enum WindType_
{
    NO_WIND,
    ACCELERATING_WIND,
    CONSTANT_WIND
};
static enum AirType_
{
    NO_AIR,
    DRY_AIR,
    HUMID_AIR,
};
namespace physics2d
{
	class PhysicsComponent
	{
	public:
        float x = 0.0f;
        float y = 0.0f;
        float mass = 0.0f;
        float density = 0.0f;
        float area = 0.0f;
        AirResistanceType_ ar_type = AR_DEFAULT;
        WindType_ wind_type = NO_WIND;
        AirType_ air_type = DRY_AIR;
        float air_temperature = 0.0f;
        float air_pressure = 0.0f;
        float air_density = 0.0f;
        float object_throw_angle = 0.0f;
        float original_wind_angle = 0.0f;
        float wind_angle = 0.0f;
        float initial_velocity = 0.0f;
        float wind_speed = 0.0f;
        float wind_acceleration = 0.0f;
        float wind_full_velocity_x = 0.0f;
        float wind_full_velocity_y = 0.0f;
        float wind_max_speed_x = 1000.0f;
        float wind_max_speed_y = 1000.0f;
        float drag_coefficent = 1.0f;
        float ar_coefficent = 1.0f;
        float terminalVelocity = 0.0f;
        float free_fall_force = 0.0f;
        float air_resistance_force = 0.0f;
        float complete_force = 0.0f;
        float timef = 0.0f;
        float height = 0.0f;
        float accel_due_to_gravity = 0.0f;
        inline void SetWindAngle(float angle_)
        {
            original_wind_angle = angle_;
            if (angle_ >= 0.0f && angle_ <= 180.0f)
            {
                angle_ += 180.0f;
            }
            else if (angle_ >= 180.0f && angle_ <= 360.0f)
            {
                angle_ -= 180.0f;
            }
            wind_angle = angle_;
        }
        inline void ApplyPhysicsComponent(float mass_, rectangle::rectf& rect_)
        {
            mass = mass_;
            x = rect_.x;
            y = rect_.y;
            area = rect_.area();
        }
        inline void ApplyPhysicsComponent(float mass_, ellipse2d::ellipsef& ellps)
        {
            mass = mass_;
            x = ellps.x;
            y = ellps.y;
            area = ellps.area();
        }
        inline void SetAirDensityParametresHeight(AirType_ AirType, float temperature_, float height_, float specific_humidity = 0.0f, float relative_humidity = 0.0f)
        {
            if (AirType == NO_WIND)
            {
                air_density = 0.0f;
            }
            if (AirType == DRY_AIR)
            {
                air_temperature = temperature_; // Measured in Kelvins!
                //thanks to this site: https://www.mide.com/air-pressure-at-altitude-calculator
                air_pressure = StaticPressure * pow(1.0f + (StandardTemperatureLapseRate / StandardTemperature * height_), (g_earth * EarthMolarMass) / (UniversalGasConstant * StandardTemperatureLapseRate));
                air_density = air_pressure / (temperature_ * UniversalGasConstantForAir);
                accel_due_to_gravity = -1.0f * (GravitationalConstant * EarthMass) / pow(EarthRadius + height_, 2);
            }
            if (AirType == HUMID_AIR)
            {
                float celsius_temp = temperature_ - 273.0f;
                float saturation_vapor_pressure = 6.1078f * pow(10, (7.5f * celsius_temp) / (celsius_temp + 273.3f));
                float vapor_pressure = relative_humidity * saturation_vapor_pressure;
                float partial_pressure_of_dry_air = (0.622f * vapor_pressure) / specific_humidity;
                accel_due_to_gravity = -1.0f * (GravitationalConstant * EarthMass) / pow(EarthRadius + height_, 2);
                air_density = (partial_pressure_of_dry_air * MolarMassOfDryAir + vapor_pressure * MolarMassOfHumidAir) / (UniversalGasConstant * temperature_);
            }
        }
        inline void ApplyPhysicsComponent(float mass_, ellipse2d::ellipsed& ellps)
        {
            mass = mass_;
            x = ellps.x;
            y = ellps.y;
            area = ellps.area();
        }
        inline void ApplyPhysicsComponent(float mass_, vector2d::fvector& vec)
        {
            mass = mass_;
            x = (vec.transform.position.x1+vec.transform.position.x2)/2;
            y = (vec.transform.position.y1 + vec.transform.position.y2) / 2;
            area = vec.transform.magnitude();
        }
        inline void ApplyPhysicsComponent(float mass_, vector2d::dvector& vec)
        {
            mass = mass_;
            x = (vec.x1+vec.x2)/2;
            y = (vec.y1 + vec.y2) / 2;
            area = vec.magnitude();
        }
        inline void ApplyPhysicsComponent(float mass_, Sprite& sprt)
        {
            mass = mass_;
            x = sprt.x;
            y = sprt.y;
            area = sprt.area_;
        }
        inline void ApplyPhysicsTransformation(rectangle::rectf& r)
        {
            r.x = x;
            r.y = y;
        }
        inline void StartPhysicsSimulation()// formula: V = gt - 1/2 * density of air * air drag coefficent * area * velocity^2*t*t/m.
		{
			//The less massive the object is, the more the force of air resistance slows the object down as it falls
            if (ar_type == NO_AR)
            {
                ar_coefficent = 0.0f;
            }
			float dt = gtime.delta_time;
            float m = mass; // we don't apply pixel_meter_ratio to mass, because it is measured in kilograms
			timef += dt;
			float y_velocity = cos(object_throw_angle*pi/180.0f)*initial_velocity/pixel_meter_ratio - (-1*accel_due_to_gravity)/pixel_meter_ratio * timef; // dividing by 100.0f is to create a propotion with pixels
            float x_velocity = sin(object_throw_angle * pi / 180.0f) *initial_velocity/pixel_meter_ratio;
            float free_fall_force_ = m * g_earth / pixel_meter_ratio;
            float apx = area;
            float a = apx/pixel_meter_ratio;
            float ar_force = 0.0f;
            float ar_velocity = 0.0f;
            float ar_acceleration = 0.0f;
            float delta_a = 0.0f;
            float free_fall_acceleration = 0.0f;
            free_fall_acceleration = y_velocity / timef;
            if (air_type != NO_AIR)
            {
                ar_force = 1.0f / 2.0f * air_density / pixel_meter_ratio * ar_coefficent * drag_coefficent * a * y_velocity * y_velocity; // when increasing ar_drag velocity increases due to minus sign in calculation of full_force variable;
                ar_velocity = (ar_force * timef) / m;
                ar_acceleration = ar_velocity / timef;
                delta_a = fabs(free_fall_acceleration) - ar_acceleration;
            }
            float x_wind_velocity = 0.0f;
            float y_wind_velocity = 0.0f;
            if (wind_type == ACCELERATING_WIND)
            {
                float x_wind_velocity_no_accel = ((sin(wind_angle * pi / 180.0f)) * (wind_speed / pixel_meter_ratio));
                float x_wind_velocity_accel = (wind_acceleration / pixel_meter_ratio) * timef;
                //x_wind_velocity = x_velocity_no_accel + x_velocity_accel;
                if (x_wind_velocity_no_accel <= 0.0f && x_wind_velocity_accel >= 0.0f)
                {
                    x_wind_velocity = x_wind_velocity_no_accel - x_wind_velocity_accel; // v=sin(angle....) * (v0+at). Just compute the velocity and then apply the angle. sin is because x is connected with sin when working with directions because sin of angles located right to the coordinate grid (0-180) is positive, but for 180-360 sin is negative
                }
                else
                {
                    x_wind_velocity = x_wind_velocity_no_accel + x_wind_velocity_accel; // v=sin(angle....) * (v0+at). Just compute the velocity and then apply the angle. sin is because x is connected with sin when working with directions because sin of angles located right to the coordinate grid (0-180) is positive, but for 180-360 sin is negative
                }
                y_wind_velocity = (cos(wind_angle * pi / 180.0f) * (wind_speed / pixel_meter_ratio)) + (wind_acceleration / pixel_meter_ratio) * timef; // same with x_wind_velocity but now everything works with cos
            }
            if (wind_type == CONSTANT_WIND)
            {
                x_wind_velocity = sin(wind_angle * pi / 180.0f) * wind_speed / pixel_meter_ratio;

                y_wind_velocity = cos(wind_angle * pi / 180.0f) * wind_speed / pixel_meter_ratio;
            }
            float full_force = 0.0f;
            float full_velocity = 0.0f;
            if (air_type != NO_AIR)
            {
                if (delta_a > 0.0f)
                {
                    full_velocity = y_velocity + ar_velocity + y_wind_velocity;
                }
                if (delta_a <= 0.0f)
                {
                    //to calculate terminal velocity g - pkAV^2/2m = 0, from here v = sqrt(2mg/pkA). So we get accelerations of two velocities(a=v/t) and if delta acceleration is negative terminal velocity is applied.
                    full_velocity = sqrt(2 * mass * accel_due_to_gravity / air_density * ar_coefficent * drag_coefficent * a); // terminal velocity
                }
            }
            float full_x_velocity = x_velocity + x_wind_velocity;
            y += full_velocity;
            x += full_x_velocity;
            density = m / a;
			free_fall_force = free_fall_force_;
            air_resistance_force = ar_force;
			complete_force = full_force;

		}
	};
}
