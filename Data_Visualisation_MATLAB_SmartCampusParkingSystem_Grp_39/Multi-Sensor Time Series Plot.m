% Multi-Sensor Time Series Plot (All 4 Sensors in One Graph)

readChannelID = 2898802;  % Your channel ID
readAPIKey = '0977080DQJSIGDX2';

% Read data for all 4 fields
[data, time] = thingSpeakRead(readChannelID, 'Fields', [1,2,3,4], 'NumPoints', 30, 'ReadKey', readAPIKey);

%% Visualize Data %%
figure;
plot(time, data(:,1), 'r-o', 'LineWidth', 1.5, 'DisplayName', 'Sensor 1');
hold on;
plot(time, data(:,2), 'g-s', 'LineWidth', 1.5, 'DisplayName', 'Sensor 2');
plot(time, data(:,3), 'b-d', 'LineWidth', 1.5, 'DisplayName', 'Sensor 3');
plot(time, data(:,4), 'm-^', 'LineWidth', 1.5, 'DisplayName', 'Sensor 4');
hold off;

xlabel('Time');
ylabel('Sensor Status (0 = Empty, 1 = Occupied)');
title('Parking Sensor Data Over Time');
legend;
grid on;
