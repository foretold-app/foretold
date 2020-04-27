[@react.component]
let make = (~seriesId: string) => {
  <SLayout head={<SLayout.TextDiv text="Edit a Series" />}>
    <ForetoldComponents.PageCard.BodyPadding>
      {SeriesGet.component(~id=seriesId, (series: option(Types.series)) =>
         switch (series) {
         | Some(series) => <SeriesForm.Edit series />
         | _ => <NotFoundPage />
         }
       )}
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};