open FC.Base;

let alerts = () => <div> <TimeSeriesChart /> </div>;

let entry = EntryTypes.(entry(~title="Alerts", ~render=alerts));