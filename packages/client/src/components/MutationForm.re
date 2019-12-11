module type Config = {type queryType;};
open Utils;

module Styles = {
  open Css;
  let warning =
    style([
      paddingTop(`em(1.)),
      paddingLeft(`em(1.)),
      paddingRight(`em(1.)),
    ]);
};

module Make = (Config: Config) => {
  let showWithLoading =
      (
        ~result: ReasonApolloTypes.mutationResponse(Config.queryType),
        ~form,
        ~successMessage="Update Successful",
        (),
      ) =>
    switch (result) {
    | Loading => <Spin />
    | Error(e) => <> {"Error: " ++ e##message |> ste} form </>
    | Data(_) => successMessage |> ste |> E.React2.inH2
    | NotCalled => form
    };

  let showWithLoading2 =
      (
        ~result: ReasonApolloTypes.mutationResponse(Config.queryType),
        ~form,
        ~onSuccess,
        (),
      ) =>
    switch (result) {
    | Loading => <Spin />
    | Error(e) =>
      <>
        <div className=Styles.warning>
          <AntdAlert message=e##message type_="warning" />
        </div>
        form
      </>
    | Data(r) => onSuccess(r)
    | NotCalled => form
    };
};