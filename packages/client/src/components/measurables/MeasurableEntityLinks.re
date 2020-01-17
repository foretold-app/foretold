module Functor = (Ken: KenTools.KenModule) => {
  let xEntityLink = (attribute, ~m: Types.measurable, ~className: string) =>
    m
    |> attribute
    |> E.O.bind(_, Ken.findName)
    |> E.O.bind(_, r =>
         m
         |> attribute
         |> E.O.fmap(_d =>
              <AntdPopover
                content={Ken.subjectIdToDisplay(
                  attribute(m) |> E.O.default(""),
                )}
                trigger=`hover
                placement=`top>
                <span className> {r |> Utils.ste} </span>
              </AntdPopover>
            )
       );

  let nameEntityLink = xEntityLink(r => r.labelSubject);
  let propertyEntityLink = xEntityLink(r => r.labelProperty);
};