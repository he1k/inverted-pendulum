function dxdt = ode1(t, y)
% dxdt(1) = dtheta
% dxdt(2) = dxc
% dxdt(3) = ddtheta
% dxdt(4) = ddxc
bp = 0.1;
bc = 2;
theta = y(1);
dtheta = y(3);
xc = y(2);
dxc = y(4);
dxdt = [y(3);
        y(4);
        (4*(- 25*cos(theta)*dtheta^2*sin(theta) + 150*bp*dtheta*sin(theta)^2 - 150*bp*dtheta + 1473*sin(theta) + 50*bc*dxc*cos(theta)))/(25*(8*cos(theta)^2 + 12*sin(theta)^2 + 1));
-(- 300*dtheta^2*cos(theta)^2*sin(theta) - 300*dtheta^2*sin(theta)^3 - 25*dtheta^2*sin(theta) + 600*bp*dtheta*cos(theta)*sin(theta)^2 - 600*bp*dtheta*cos(theta) + 600*bc*dxc*cos(theta)^2 + 5892*cos(theta)*sin(theta) + 600*bc*dxc*sin(theta)^2 + 50*bc*dxc)/(150*(8*cos(theta)^2 + 12*sin(theta)^2 + 1))

];


end