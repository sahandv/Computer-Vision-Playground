%% Code by sahand 2018
%% Task 1 :
% Load TEAPOT
obj = readObj('wt_teapot.obj');
v1_1 = obj.v;
f1_1 = obj.f.v;

vf.vertices = v1_1;
vf.faces = f1_1;

figure;cla
pt = patch( vf ,'FaceColor','red','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
title('Combined Plots: rotation over Y. (red = original)')

hold on

% Rotation Matrix inputs
t_1 = deg2rad(45);
w_x = 0;
w_y = 1;
w_z = 0;

% Rotation along Y axes (simple method)
R1_w = [cos(t_1) 0 sin(t_1) ; 0 1 0 ; -sin(t_1) 0  cos(t_1)];

% OR use custom Rodrigues method (function has been used for reusability)
R1_w = Rodrigues( t_1,w_x,w_y,w_z );

% Rotate TEAPOT around Y axes
v1_2 = R1_w*transpose(v1_1);
v1_2 = transpose(v1_2);

vf.vertices = v1_2;

pt = patch( vf ,'FaceColor','blue','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud

% Show rotation vector
p1 = v1_1(150,:);
p2 = v1_2(150,:);
x0 = p1(1);
y0 = p1(2);
z0 = p1(3);
x1 = p2(1);
y1 = p2(2);
z1 = p2(3);
h1 = plot3([x0;x1],[y0;y1],[z0;z1],'green');   % Draw a line between p1 and p2
set(h1(1),'linewidth',3);
p = p2-p1;
alpha = 0.3;  % Size of arrow head relative to the length of the vector
beta = 0.5;  % Width of the base of the arrow head relative to the length
hu = [x1-alpha*(p(1)+beta*(p(2)+eps)); x1; x1-alpha*(p(1)-beta*(p(2)+eps))];
hv = [y1-alpha*(p(2)-beta*(p(1)+eps)); y1; y1-alpha*(p(2)+beta*(p(1)+eps))];
hw = [z1-alpha*p(3);z1;z1-alpha*p(3)];
h2 = plot3(hu(:),hv(:),hw(:),'green');  % Plot arrow head
set(h2(1),'linewidth',3);
hold off
% Rotate TEAPOT around the axis: w2 = [0.3, 0.7, -1] Using norm of the axis
w1_2 = [0.3, 0.7, -1];
w1_2_unit = w1_2/norm(w1_2);
R1_w2 = Rodrigues( norm(w1_2),w1_2_unit(1),w1_2_unit(2),w1_2_unit(3) );
v1_3 = R1_w2*transpose(v1_1);
v1_3 = transpose(v1_3);

vf.vertices = v1_1;

figure;cla
pt = patch( vf ,'FaceColor','red','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
title('Combined Plots: rotation over  w2 = [0.3, 0.7, -1]. (red = original)')
hold on
vf.vertices = v1_3;
pt = patch( vf ,'FaceColor','blue','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
hold off

%% Task2 : 
% Calc rotation matrices R2_w1 and R2_w2
w2_1 = [1, 0.5, 0];
w2_2 = [0.3, 0.7, -1];

w2_1_unit = w2_1/norm(w2_1);
w2_2_unit = w2_2/norm(w2_2);

R2_w1 = Rodrigues( norm(w2_1),w2_1_unit(1),w2_1_unit(2),w2_1_unit(3) );
R2_w2 = Rodrigues( norm(w2_2),w2_2_unit(1),w2_2_unit(2),w2_2_unit(3) );

% Rotate TEAPOT around w1 and w2 and make v2_1
v2_1 = R2_w1*transpose(v1_1);
v2_1 = transpose(v2_1);

v2_1 = R2_w2*transpose(v2_1);
v2_1 = transpose(v2_1);

% Rotate TEAPOT around w2 and w1 and make v2_2
v2_2 = R2_w2*transpose(v1_1);
v2_2 = transpose(v2_2);

v2_2 = R2_w1*transpose(v2_2);
v2_2 = transpose(v2_2);

% Compare and display rotation v2_1 and v2_2 and check commutativity
vf.vertices = v2_1;
figure;cla
pt = patch( vf ,'FaceColor','green','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
title('Task 2 : check if commutative: NOT commutative')
hold on
vf.vertices = v2_2;
pt = patch( vf ,'FaceColor','blue','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
hold off

%% Task3 :
% Selection of w3_1 to w3_6, Assuming w = [0.3, 0.7, -1]
w3_1 = [0.3, 0.7, -1];
w3_2 = [-0.3, 0.7, -1];
w3_3 = [0.3, -0.7, -1];
w3_4 = [-0.3, 0.7, 1];
w3_5 = [0.3, -0.7, 1];
w3_6 = [-0.3, -0.7, 1];

% Generating rotation matrices for w3_i
w3_1_unit = w3_1/norm(w3_1);
w3_2_unit = w3_2/norm(w3_2);
w3_3_unit = w3_3/norm(w3_3);
w3_4_unit = w3_4/norm(w3_4);
w3_5_unit = w3_5/norm(w3_5);
w3_6_unit = w3_6/norm(w3_6);

R3_w1 = Rodrigues( norm(w3_1),w3_1_unit(1),w3_1_unit(2),w3_1_unit(3) );
R3_w2 = Rodrigues( norm(w3_2),w3_2_unit(1),w3_2_unit(2),w3_2_unit(3) );
R3_w3 = Rodrigues( norm(w3_3),w3_3_unit(1),w3_3_unit(2),w3_3_unit(3) );
R3_w4 = Rodrigues( norm(w3_4),w3_4_unit(1),w3_4_unit(2),w3_4_unit(3) );
R3_w5 = Rodrigues( norm(w3_5),w3_5_unit(1),w3_5_unit(2),w3_5_unit(3) );
R3_w6 = Rodrigues( norm(w3_6),w3_6_unit(1),w3_6_unit(2),w3_6_unit(3) );

% Rotation of TEAPOT using R3_wi (6 rotations)
v3_1 = R3_w1*transpose(v1_1);
v3_1 = transpose(v3_1);

v3_2 = R3_w2*transpose(v1_1);
v3_2 = transpose(v3_2);

v3_3 = R3_w3*transpose(v1_1);
v3_3 = transpose(v3_3);

v3_4 = R3_w4*transpose(v1_1);
v3_4 = transpose(v3_4);

v3_5 = R3_w5*transpose(v1_1);
v3_5 = transpose(v3_5);

v3_6 = R3_w6*transpose(v1_1);
v3_6 = transpose(v3_6);

% Displaying of 5 different rotations
vf.vertices = v3_1;
figure;cla
pt = patch( vf ,'FaceColor','green','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
title('Task 3')
hold on

vf.vertices = v3_2;
pt = patch( vf ,'FaceColor','blue','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud

vf.vertices = v3_3;
pt = patch( vf ,'FaceColor','yellow','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud

vf.vertices = v3_4;
pt = patch( vf ,'FaceColor','cyan','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud

vf.vertices = v3_5;
pt = patch( vf ,'FaceColor','red','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud

vf.vertices = v3_6;
pt = patch( vf ,'FaceColor','magenta','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud

hold off

%% Task 4 :
% Get quaternion representations
Q1_x = w2_1_unit(1)*sin(norm(w2_1)/2);
Q1_y = w2_1_unit(2)*sin(norm(w2_1)/2);
Q1_z = w2_1_unit(3)*sin(norm(w2_1)/2);
Q1_w = cos(norm(w2_1)/2);

Q2_x = w2_2_unit(1)*sin(norm(w2_2)/2);
Q2_y = w2_2_unit(2)*sin(norm(w2_2)/2);
Q2_z = w2_2_unit(3)*sin(norm(w2_2)/2);
Q2_w = cos(norm(w2_2)/2);

% Compute rotation matrix
R4_Q1 = QuaternionRotationMatrix(Q1_x,Q1_y,Q1_z,Q1_w);
R4_Q2 = QuaternionRotationMatrix(Q2_x,Q2_y,Q2_z,Q2_w);
% Alternatively, matlab also has a code for doing: QuaternionRotationMatrix

% Rotate with quaternions
v4_1 = R4_Q1*transpose(v1_1);
v4_1 = transpose(v4_1);
v4_1 = R4_Q2*transpose(v4_1);
v4_1 = transpose(v4_1);

v4_2 = R4_Q2*transpose(v1_1);
v4_2 = transpose(v4_2);
v4_2 = R4_Q1*transpose(v4_2);
v4_2 = transpose(v4_2);

% Compare and display rotation v4_1 and v4_2 and check commutativity
vf.vertices = v4_1;
figure;cla
pt = patch( vf ,'FaceColor','green','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
title('Task 4 : check if commutative: NOT commutative')
hold on
vf.vertices = v4_2;
pt = patch( vf ,'FaceColor','blue','EdgeColor','none','FaceAlpha',0.5);
daspect([1,1,1]) ; view(-3,4) ; axis tight ; camlight ; lighting gouraud
hold off

