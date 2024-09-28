import pandas as pd
import matplotlib as plt
import numpy as np
from flask_ngrok import run_with_ngrok
from flask import Flask, request, render_template
import pickle

dataset=pd.read_csv('TableData6.csv')
dataset1=pd.read_excel('ECCO4.xlsx')

dataset['Time'] = pd.to_datetime(dataset['Time'])
dataset1['DateTime'] = pd.to_datetime(dataset1['DateTime'])
dataset['Time'] = dataset['Time'].dt.round('10min')
dataset1['DateTime'] = dataset1['DateTime'].dt.round('10min')

dataset.rename(columns={'Time': 'DateTime'},inplace=True)
merged_data = pd.merge(dataset, dataset1, on='DateTime', how='inner')
merged_data.head()
print(len(merged_data))

dataset = merged_data[merged_data['RT'] != 0].copy()

dataset['DateTime'] = pd.to_datetime(dataset['DateTime'])
dataset['Is_Weekend'] = dataset['DateTime'].dt.weekday.apply(lambda x: 1 if x >= 5 else 0)
print(dataset)

X=dataset.loc[:, dataset.columns.isin(['GPM', 'CHWS','CHWR','kW_RT','Temperature [°C]', 'kW_CHH', 'RH [%]','WBT_C'])]
Y=dataset.iloc[:,dataset.columns=='CH Load']
print(Y)


from sklearn.model_selection import train_test_split
import pandas as pd
import numpy as np
x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=0.2)#, #random_state=0)
'''
split_index = int(len(dataset) * 0.8)
train_data = dataset.iloc[:split_index]
test_data = dataset.iloc[split_index:]
X_columns = dataset.columns.difference(['CH Load'])
y_column = 'CH Load'
x_train = train_data[X_columns]
y_train = train_data[y_column]

x_test = test_data[X_columns]
y_test = test_data[y_column]

x_train = x_train.apply(pd.to_numeric, errors='coerce')
x_test = x_test.apply(pd.to_numeric, errors='coerce')
'''
from sklearn.svm import SVR
from sklearn.metrics import accuracy_score, r2_score, mean_squared_error
svr_model = SVR(kernel='rbf')
svr_model.fit(x_train, y_train.values.ravel())
ypred_svr = svr_model.predict(x_test)
r2_svr = r2_score(y_test, ypred_svr)
mse_svr = mean_squared_error(y_test, ypred_svr)
print("SVR R² Score: ", r2_svr * 100, "%")
print("SVR Mean Squared Error: ", mse_svr)
print("Training data shape:", x_train.shape)
print("Testing data shape:", x_test.shape)
'''
print("Accuracy: ", a1*100, "%")
mse=mean_squared_error(y_test, ypred1)
print("Mean square error:", mse)
print("Training data shape:", x_train.shape)
print("Testing data shape:", x_test.shape)
'''
pickle.dump(svr_model, open('model.pkl', 'wb'))
