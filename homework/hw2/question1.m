raw_train = dlmread('wine_training1.txt',',');
x_train1 = raw_train(:,2:14);
y_train1 = raw_train(:,1);
x_train1 = (x_train1 - (ones(length(y_train1),1)* mean(x_train1)))./(ones(length(y_train1),1)*std(x_train1, 0, 1));

raw_train = dlmread('wine_training2.txt',',');
x_train2 = raw_train(:,2:14);
y_train2 = raw_train(:,1);
x_train2 = (x_train2 - (ones(length(y_train2),1)* mean(x_train2)))./(ones(length(y_train2),1)*std(x_train2, 0, 1));

raw_train = dlmread('wine_testing.txt',',');
x_test = raw_train(:,2:14);
y_test = raw_train(:,1);
x_test = (x_test - (ones(length(y_test),1)* mean(x_test)))./(ones(length(y_test),1)*std(x_test, 0, 1));

lambda = [.0001, .0005, .0025, .0125, .0625, .3125, 1.5625, 7.815, 39.0625, 195.3125];
ita = 10^-3;
w = ones(13,1);

x_trains{1} = x_train1;
x_trains{2} = x_train2;
y_trains{1} = y_train1;
y_trains{2} = y_train2;
e_ins = [];
e_outs = [];
w_norms = [];
for i = 1:2
    x_train = x_trains{i};
    y_train = y_trains{i};
    [row, col] = size(x_train);
    [row_t, col_t] = size(x_test);
    e_in = zeros(10,1);
    e_out = zeros(10,1);
    norm_list = zeros(10,1);
    
    for n = 1:10
        w = zeros(13,1);
        gra = zeros(13,1);
        tt=zeros(10^7,1);
        for j=1:row
            tt(1)=tt(1)+log(1+exp(-y_train(j,1)*x_train(j,:)*w));
        end
        tt(1)=tt(1)+(lambda(n)/row)*w'*w;
        
        gra=((ones(1,row)-1./(1+exp(-y_train'.*(w'*x_train')))).*y_train'*x_train)'+2*(lambda(n)/row)*w;
        w=w+10^-4*gra;
        for j=1:row
            tt(2)=tt(2)+log(1+exp(-y_train(j,1)*x_train(j,:)*w));
        end
        tt(2)=tt(2)+(lambda(n)/row)*w'*w;
        k=2;

        while (tt(k-1)-tt(k))>=ita*(tt(1)-tt(2))
            gra=((ones(1,row)-1./(1+exp(-y_train'.*(w'*x_train')))).*y_train'*x_train)'+2*(lambda(n)/row)*w;
            w=w+10^-4*gra;
            k=k+1;
        
            for j=1:row
                tt(k)=tt(k)+log(1+exp(-y_train(j,1)*x_train(j,:)*w));
            end
            tt(k)=tt(k)+(lambda(n)/row)*w'*w;
               
         end      

       e_in(n)=0;
       e_out(n)=0;
       norm_list(n) = norm(w);

       for j=1:row
            e_in(n)=e_in(n)+log(1+exp(-y_train(j,1)*x_train(j,:)*w));
       end
       for j=1:row_t
            e_out(n)=e_out(n)+log(1+exp(-y_test(j,1)*x_test(j,:)*w));
       end

       
   end
    e_ins{i} = e_in;
    e_outs{i} = e_out;
    w_norms{i} = norm_list;
end

figure(1)
for i = 1:2
    semilogx(lambda, e_ins{i});
    hold on
end
xlabel('Lambda');
ylabel('E_{in}');
title('E_{in} vs Lambda');
l = legend('Wine Dataset 1', 'Wine Dataset 2');

figure(2)
for i = 1:2
    semilogx(lambda, e_outs{i});
    hold on
end
xlabel('Lambda');
ylabel('E_{out}');
title('E_{out} vs Lambda');
l = legend('Wine Dataset 1', 'Wine Dataset 2');

figure(3)
for i = 1:2
    semilogx(lambda, w_norms{i});
    hold on
end
xlabel('Lambda');
ylabel('Norm of w');
title('Norm of w vs Lambda');
l = legend('Wine Dataset 1', 'Wine Dataset 2');

