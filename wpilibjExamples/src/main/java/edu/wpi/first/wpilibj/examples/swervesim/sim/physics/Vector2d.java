package frc.sim.physics;

/**
 * This is a 2D vector struct that supports basic vector operations.
 */
@SuppressWarnings("MemberName")
public class Vector2d extends edu.wpi.first.wpilibj.drive.Vector2d {

    public Vector2d(){
        super(0.0,0.0);
    }

    public Vector2d(double x, double y){
        super(x,y);
    }

    public double cross(Vector2d other){
        return this.x*other.y - this.y*other.x;
    }
}
