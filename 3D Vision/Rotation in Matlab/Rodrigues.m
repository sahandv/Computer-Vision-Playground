function [ R_w_t_Rodrigues ] = Rodrigues( t,w_x,w_y,w_z )
%RODRIGUES 
% Rodrigues formula

    m_11 = cos(t)+(w_x.^2)*(1-cos(t));
    m_12 = w_x*w_y*(1-cos(t))-w_z*sin(t);
    m_13 = w_y*sin(t)+w_x*w_z*(1-cos(t));

    m_21 = w_z*sin(t)+w_x*w_y*(1-cos(t));
    m_22 = cos(t)+(w_y.^2)*(1-cos(t));
    m_23 = -w_x*sin(t)+w_y*w_z*(1-cos(t));

    m_31 = -w_y*sin(t)+w_x*w_z*(1-cos(t));
    m_32 = w_x*sin(t)+w_y*w_z*(1-cos(t));
    m_33 = cos(t)+(w_z.^2)*(1-cos(t));

    R_w_t_Rodrigues = [m_11,m_12,m_13;m_21,m_22,m_23;m_31,m_32,m_33];

end

