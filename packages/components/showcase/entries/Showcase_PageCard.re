open FC;

let render = () =>
  <PageCard.Jsx2>
    <PageCard.HeaderRow.Jsx2>
      <PageCard.HeaderRow.Title.Jsx2>
        "PageCard.HeaderRow.Title"->React.string
      </PageCard.HeaderRow.Title.Jsx2>
    </PageCard.HeaderRow.Jsx2>
    <PageCard.BodyPadding.Jsx2>
      <PageCard.H1> "PageCard.H1"->React.string </PageCard.H1>
      <PageCard.P> "PageCard.P"->React.string </PageCard.P>
    </PageCard.BodyPadding.Jsx2>
    <PageCard.Section border=`top background=`grey>
      "Section, grey + borderTop"->React.string
    </PageCard.Section>
  </PageCard.Jsx2>;

let entry = EntryTypes.(entry(~title="PageCard", ~render));
