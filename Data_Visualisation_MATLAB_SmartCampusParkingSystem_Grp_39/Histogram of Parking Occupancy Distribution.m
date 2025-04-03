% Histogram of Parking Occupancy Distribution

readChannelID = 2898802;  
readAPIKey = '0977080DQJSIGDX2';

[data, time] = thingSpeakRead(readChannelID, 'Fields', [1,2,3,4], 'NumPoints', 100, 'ReadKey', readAPIKey);

data(isnan(data)) = 0;

figure;
histogram(sum(data, 2), 'FaceColor', 'r');
xlabel('Number of Occupied Spaces');
ylabel('Frequency');
title('Histogram of Parking Occupancy');
grid on;
