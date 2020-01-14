module RemoveNotebookButton = {
  [@react.component]
  let make = (~notebookId: string) => {
    <NotebookDelete.Mutation onCompleted={_ => Js.log("ss")}>
      ...{(mutation, result: NotebookDelete.Mutation.renderPropObj) =>
        <Antd.Popconfirm
          title={Lang.notebookRemove |> Utils.ste}
          onConfirm={_ => NotebookDelete.mutate(mutation, notebookId)}>
          <Antd.Button loading={Bool(result.loading)} _type=`danger>
            {"Delete" |> Utils.ste}
          </Antd.Button>
        </Antd.Popconfirm>
      }
    </NotebookDelete.Mutation>;
  };
};