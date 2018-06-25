#ifndef MAIN_H
#define MAIN_H


enum eLevel {       //TODO: figure out how to declare this only once (now in logger.h and main.h)
    FATAL = 0x00, ERROR=0x01, WARN=0x02, INFO=0x04, WATCH=0x08, DEBUG=0x10,
    UART=0x100, COMMAND=0x200, NETWORK=0x400, DATA=0x800, ALL=0xff00
};

#endif // MAIN_H

/*

**** Object:  Table [dbo].[logControl]    Script Date: 10-6-2018 18:31:40 ****
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[logControl](
    [id] [bigint] IDENTITY(1,1) NOT NULL,
    [timestamp] DATETIME NOT NULL DEFAULT(GETDATE()),
    [level] [smallint] NULL,
    [message] [ntext] NULL,
 CONSTRAINT [PK_logControl] PRIMARY KEY CLUSTERED
(
    [id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO


****** Object:  Table [dbo].[currentState]    Script Date: 14-6-2018 17:02:01 ******
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[currentState](
    [id] [bigint] IDENTITY(1,1) NOT NULL,
    [timestamp] DATETIME NOT NULL DEFAULT(GETDATE()),
    [config] [ntext] NULL,
    [state] [ntext] NULL,
     CONSTRAINT [PK_currentState] PRIMARY KEY CLUSTERED
(
    [id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO


//show log with error level
SELECT distinct convert(varchar(100), message), (level & 1), count((level&1))
  FROM [mttf].[dbo].[logControl]
  group by convert(varchar(100), message), (level & 1)
  having (level & 1)=1
  ;

select * from logControl where level=0;

//select state without newlines
SELECT [id]
      ,[timestamp]
      ,REPLACE(convert(varchar(255),[state]),CHAR(10),'')
  FROM [mttf].[dbo].[currentState]
  order by id desc;






*/
