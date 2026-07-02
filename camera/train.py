import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
import joblib


def train(src_filepath): 

    df = pd.read_csv(src_filepath)

    X = df.drop(["filename", "label"], axis=1)
    y = df["label"]

    le = LabelEncoder()
    y = le.fit_transform(y)

    X_train, X_test, y_train, y_test = train_test_split(
        X,
        y,
        test_size=0.2,
        random_state=10
    )

    model = RandomForestClassifier(
        n_estimators=100,
        random_state=10
    )

    model.fit(X_train, y_train)

    predictions = model.predict(X_test)

    print("Accuracy:", accuracy_score(y_test, predictions))
    print()

    print(classification_report(y_test, predictions))
    print()

    print(confusion_matrix(y_test, predictions))

    joblib.dump(model, "random_forest_model.pkl")

def main():

    filename = "dataset.csv"

    print(f"Training using {filename}...")
    print()
    train(filename)
    print()
    print("Model exported as 'random_forest_model.pkl'.")

if __name__ == "__main__":
    main()