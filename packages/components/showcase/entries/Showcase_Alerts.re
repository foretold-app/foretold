open FC.Base;

let alerts = () =>
  <div>
    <Alert.Jsx2 type_=`primary> "Primary alert"->React.string </Alert.Jsx2>
    <Alert.Jsx2 type_=`info> "Info alert"->React.string </Alert.Jsx2>
    <Alert.Jsx2 type_=`success> "Success alert"->React.string </Alert.Jsx2>
    <Alert.Jsx2 type_=`warning> "Warning alert"->React.string </Alert.Jsx2>
    <Alert.Jsx2 type_=`error> "Error alert"->React.string </Alert.Jsx2>
  </div>;

let entry = EntryTypes.(entry(~title="Alerts", ~render=alerts));
