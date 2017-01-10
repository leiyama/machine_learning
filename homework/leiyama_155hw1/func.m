function [ w, e ] = func( X,Y,w0,mu)

% mu=2.718^(-15); 
w=w0;
e=zeros(100,1);
los1=(X*w0-Y)'*(X*w0-Y);
for j=1:1000
   m=randi(1000);
   w=w-mu*2*(X(m,:)*w-Y(m))*X(m,:)';
end    
los2=(X*w-Y)'*(X*w-Y);
e(1)= los2;
lr0=los1-los2;
los1=los2;
lr=lr0;
i=2;
while  lr/lr0>0.0001 && i<=100
    for j=1:1000
     m=randi(1000);
     w=w-mu*2*(X(m,:)*w-Y(m))*X(m,:)';
    end  
    los2=(X*w-Y)'*(X*w-Y);
    e(i)=los2;
    lr=los1-los2;
    los1=los2;
    i=i+1;
end

end

