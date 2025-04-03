% Moving Average of Parking Occupancy

readChannelID = 2898802;  
readAPIKey = '0977080DQJSIGDX2';

[data, time] = thingSpeakRead(readChannelID, 'Fields', [1,2,3,4], 'NumPoints', 50, 'ReadKey', readAPIKey);

data(isnan(data)) = 0;

% Compute moving average
windowSize = 5; % Adjust window size for smoothing
smoothOccupancy = movmean(sum(data, 2), windowSize);

figure;
plot(time, smoothOccupancy, 'g-', 'LineWidth', 1.5);
xlabel('Time');
ylabel('Smoothed Occupancy');
title('Moving Average of Parking Usage');
grid on;
