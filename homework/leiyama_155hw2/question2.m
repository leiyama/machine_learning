data = dlmread('question2data.txt', ',');
X = data(:, 1:9);
Y = data(:, 10);

k = .05;
a = ridge(Y, X, k, 0);
b = (X' * X + k * eye(9)) \ X' * Y;

lambdas = transpose(linspace(0, 1.7, 50));
w1 = lasso(X, Y, 'Lambda', lambdas, 'Standardize', false);

% data = ['-y'; '-m'; '-c'; '-r'; '-g'; '-b'; '-g'; '-k'; '-m'];
% celldata = cellstr(data);

figure(1)
for i = 1:size(w1, 1)
    plot(lambdas, w1(i,:));
    hold on
end

xlabel('Lambda');
ylabel('Weights');
title('Weights vs Lambda (Lasso Regularization)');
l = legend('w(1)', 'w(2)', 'w(3)', 'w(4)', 'w(5)', 'w(6)', 'w(7)', 'w(8)', 'w(9)');

figure(2)
lambdas = transpose(linspace(0, 5000, 50));
w2 = [];
for i = 1:size(lambdas, 1)
    w2 = [w2, inv(X.'*X+eye(size(X, 2))*lambdas(i))*X.'*Y]; 
end
for i = 1:size(w2, 1)
    plot(lambdas, w2(i,:));
    hold on
end

xlabel('Lambda');
ylabel('Weights');
title('Weights vs Lambda (Ridge Regularization)');
l = legend('w(1)', 'w(2)', 'w(3)', 'w(4)', 'w(5)', 'w(6)', 'w(7)', 'w(8)', 'w(9)');

sols = w1';
idx = sols~=0;
num_zeros = sum(idx, 2);
nines = ones(size(num_zeros, 1), 1) * 9;
num_zeros = nines - num_zeros;
lambdas = transpose(linspace(0, 1.7, 50));
figure(3)
plot(lambdas, num_zeros, '-r');


xlabel('Lambda');
ylabel('Number of Zero Weights');
title('Number of Zero Weights vs Lambda (Lasso Regularization)');