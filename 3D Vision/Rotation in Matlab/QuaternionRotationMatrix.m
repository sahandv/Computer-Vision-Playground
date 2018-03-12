function [ matrix ] = QuaternionRotationMatrix( qx,qy,qz,qw )
%QUATERNIONROTATIONMATRIX 

    r_11 = 1-2*(qy.^2)-2*qz.^2;
    r_12 = 	2*qx*qy - 2*qz*qw;
    r_13 = 2*qx*qz + 2*qy*qw;

    r_21 = 2*qx*qy + 2*qz*qw;
    r_22 = 1 - 2*qx.^2 - 2*qz.^2;
    r_23 = 2*qy*qz - 2*qx*qw;

    r_31 = 2*qx*qz - 2*qy*qw;
    r_32 = 2*qy*qz + 2*qx*qw;
    r_33 = 1 - 2*qx.^2 - 2*qy.^2;

    matrix = [r_11,r_12,r_13;r_21,r_22,r_23;r_31,r_32,r_33];
    
end

