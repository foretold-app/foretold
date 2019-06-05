open FC.Base;

let alerts = () =>
  <div>
    <Alert type_=Colors.Alert.Primary> "Primary alert"->React.string </Alert>
    <Alert type_=Colors.Alert.Info> "Info alert"->React.string </Alert>
    <Alert type_=Colors.Alert.Success> "Success alert"->React.string </Alert>
    <Alert type_=Colors.Alert.Warning> "Warning alert"->React.string </Alert>
    <Alert type_=Colors.Alert.Error> "Error alert"->React.string </Alert>
  </div>;

let entry = EntryTypes.(entry(~title="Alerts", ~render=alerts));