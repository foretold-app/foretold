open FC.Base;

let alerts = () =>
  <div>
    <Alert type_=Alert.Primary> "Primary alert"->React.string </Alert>
    <Alert type_=Alert.Info> "Info alert"->React.string </Alert>
    <Alert type_=Alert.Success> "Success alert"->React.string </Alert>
    <Alert type_=Alert.Warning> "Warning alert"->React.string </Alert>
    <Alert type_=Alert.Error> "Error alert"->React.string </Alert>
  </div>;

let entry = EntryTypes.[entry(~title="Alerts", ~render=alerts)];
