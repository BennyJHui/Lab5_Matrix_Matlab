% Name: Benny Hui, Rishi Singh
% Student ID: 29738390, 17722430
% CWL: Bhui05, rishisin

ConnectivityMatrix = [0 1 0 1 0; 1 0 0 1 1; 1 0 0 1 0; 1 1 0 0 1; 1 0 0 0 0]; % Create connectivity Matrix
[rows, columns] = size(ConnectivityMatrix); % Gets the dimensions of the matrix
dimension = size(ConnectivityMatrix, 1); % Gets the dimension size
columnsums = sum(ConnectivityMatrix, 1); % Sums up each column
p = 0.85; % Probability factor
zerocolumns = find(columnsums~=0); % Finds indices of column which sum is not zero
D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension); % Creates a sparse matrix
StochasticMatrix = ConnectivityMatrix * D; % Create stochastic matrix
[row, column] = find(columnsums==0); % Find the zero column of original connectivity matrix
StochasticMatrix(:, column) = 1./dimension; % Create Stochastic Matrix
Q = ones(dimension, dimension); % Generates transition matrix
TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension); % Create transition matrix
PageRank = ones(dimension, 1);
for i = 1:100; PageRank = TransitionMatrix * PageRank; end
PageRank = PageRank / sum(PageRank); % Stores results into "PageRank"